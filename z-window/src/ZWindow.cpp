#include "z-window/include/ZWindow.h"

#include <GLES3/gl3.h>
#include <emscripten/html5.h>

#include <cstdio>

#include "include/core/SkCanvas.h"
#include "include/core/SkColor.h"
#include "include/core/SkColorSpace.h"
#include "include/core/SkSurface.h"
#include "include/gpu/ganesh/GrBackendSurface.h"
#include "include/gpu/ganesh/GrDirectContext.h"
#include "include/gpu/ganesh/GrTypes.h"
#include "include/gpu/ganesh/SkSurfaceGanesh.h"
#include "include/gpu/ganesh/gl/GrGLBackendSurface.h"
#include "include/gpu/ganesh/gl/GrGLDirectContext.h"
#include "include/gpu/ganesh/gl/GrGLInterface.h"
#include "include/gpu/ganesh/gl/GrGLMakeWebGLInterface.h"
#include "src/gpu/ganesh/gl/GrGLDefines.h"
#include "z-paint/include/z-document-painter.h"
#include "z-paint/include/z-overlay-painter.h"
#include "z-tools/include/z-editor-theme.h"

namespace {

constexpr const char* kCanvasSelector = "#canvas";

}  // namespace

struct ZWindowSkiaState {
    EMSCRIPTEN_WEBGL_CONTEXT_HANDLE webglContext{0};
    sk_sp<GrDirectContext> directContext{nullptr};
    sk_sp<SkSurface> surface{nullptr};
    int surfaceWidth{0};
    int surfaceHeight{0};
};

ZWindow::ZWindow() {
    init();
}

ZWindow::~ZWindow() {
    destroySurface();
}

void ZWindow::init() {
    zSkia = std::make_unique<ZWindowSkiaState>();
    ensureSurface();
    zDocumentPainter = std::make_shared<ZDocumentPainter>();
    zOverlayPainter = std::make_shared<ZOverlayPainter>(zEditorContext);
}

void ZWindow::destroySurface() {
    if (!zSkia) {
        return;
    }

    if (zSkia->directContext) {
        zSkia->directContext->releaseResourcesAndAbandonContext();
    }
    zSkia->surface.reset();
    zSkia->directContext.reset();

    if (zSkia->webglContext != 0) {
        emscripten_webgl_destroy_context(zSkia->webglContext);
        zSkia->webglContext = 0;
    }
}

void ZWindow::ensureSurface() {
    if (zWidth <= 0 || zHeight <= 0) {
        return;
    }

    if (!zSkia) {
        zSkia = std::make_unique<ZWindowSkiaState>();
    }

    if (zSkia->webglContext == 0) {
        EmscriptenWebGLContextAttributes attrs;
        emscripten_webgl_init_context_attributes(&attrs);
        attrs.alpha = EM_TRUE;
        attrs.depth = EM_FALSE;
        attrs.stencil = EM_TRUE;
        attrs.antialias = EM_TRUE;
        attrs.majorVersion = 2;
        attrs.minorVersion = 0;
        attrs.enableExtensionsByDefault = EM_TRUE;

        zSkia->webglContext = emscripten_webgl_create_context(kCanvasSelector, &attrs);
        if (zSkia->webglContext <= 0) {
            printf("ZWindow: emscripten_webgl_create_context failed: %d\n",
                   static_cast<int>(zSkia->webglContext));
            zSkia->webglContext = 0;
            return;
        }
    }

    emscripten_webgl_make_context_current(zSkia->webglContext);
    emscripten_set_canvas_element_size(kCanvasSelector, zWidth, zHeight);

    if (!zSkia->directContext) {
        auto interface = GrGLInterfaces::MakeWebGL();
        zSkia->directContext = GrDirectContexts::MakeGL(interface);
        if (!zSkia->directContext) {
            printf("ZWindow: GrDirectContexts::MakeGL failed\n");
            return;
        }
    }

    if (zSkia->surface && zSkia->surfaceWidth == zWidth && zSkia->surfaceHeight == zHeight) {
        return;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, zWidth, zHeight);
    zSkia->directContext->resetContext(kRenderTarget_GrGLBackendState | kMisc_GrGLBackendState);

    GrGLFramebufferInfo framebufferInfo;
    framebufferInfo.fFBOID = 0;
    framebufferInfo.fFormat = GR_GL_RGBA8;

    auto renderTarget = GrBackendRenderTargets::MakeGL(zWidth, zHeight, 0, 8, framebufferInfo);
    zSkia->surface = SkSurfaces::WrapBackendRenderTarget(
        zSkia->directContext.get(), renderTarget, kBottomLeft_GrSurfaceOrigin,
        kRGBA_8888_SkColorType, SkColorSpace::MakeSRGB(), nullptr);
    if (!zSkia->surface) {
        printf("ZWindow: SkSurfaces::WrapBackendRenderTarget failed\n");
        return;
    }

    zSkia->surfaceWidth = zWidth;
    zSkia->surfaceHeight = zHeight;
}

void ZWindow::draw() {
    ensureSurface();
    if (!zSkia || !zSkia->surface || !zSkia->directContext || zSkia->webglContext == 0) {
        return;
    }

    emscripten_webgl_make_context_current(zSkia->webglContext);
    auto* canvas = zSkia->surface->getCanvas();

    const auto backgroundColor = ZEditorTheme::GetColor(ZEditorThemeToken::zCanvasBackground);
    canvas->clear(SkColorSetRGB((backgroundColor >> 16) & 0xFF, (backgroundColor >> 8) & 0xFF,
                                backgroundColor & 0xFF));

    canvas->save();
    canvas->scale(zDpr, zDpr);

    if (zDocumentPainter) {
        zDocumentPainter->draw(canvas);
    }

    if (zOverlayPainter) {
        zOverlayPainter->draw(canvas);
    }

    canvas->restore();

    present();
}

void ZWindow::present() {
    if (!zSkia || !zSkia->surface || !zSkia->directContext) {
        return;
    }

    skgpu::ganesh::FlushAndSubmit(zSkia->surface);
    glFlush();
}

void ZWindow::setPage(const z_sp<ZPage>& page) {
    zDocumentPainter->setPage(page);
}

void ZWindow::setEditorContext(ZEditorContext* context) {
    zEditorContext = context;
    zOverlayPainter = std::make_shared<ZOverlayPainter>(zEditorContext);
}

void ZWindow::setOverlayRoot(z_sp<ZShape> root) {
    zOverlayPainter->setRoot(std::move(root));
}

void ZWindow::setTrace(ZTrace* trace) {
    zOverlayPainter->setTrace(trace);
}

void ZWindow::dump() const {
    printf("window[%p]\n", this);
}

void ZWindow::setContext(const WindowContext& context) {
    if (context.zWidth == 0 || context.zHeight == 0 || context.zPixelWidth == 0 ||
        context.zPixelHeight == 0 || context.zDpr <= 0.0f) {
        return;
    }

    zCssWidth = static_cast<float>(context.zWidth);
    zCssHeight = static_cast<float>(context.zHeight);
    zDpr = context.zDpr;
    zWidth = static_cast<int>(context.zPixelWidth);
    zHeight = static_cast<int>(context.zPixelHeight);
    ensureSurface();
}
