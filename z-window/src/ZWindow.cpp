#include "z-window/include/ZWindow.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>  // 建议加上，确保 GL 宏定义完整
#include <SDL2/SDL_video.h>   // 必须加上这个，它定义了 GLContext 和相关的操作函数

#include <algorithm>
#include <iostream>
#include <thread>
#include <utility>

#include "z-engine/libs/nanovg/nanovg.h"
#include "z-paint/include/z-document-painter.h"
#include "z-paint/include/z-overlay-painter.h"
#include "z-window/include/z-bitmap-surface.h"
#include "z-window/include/z-canvas-surface.h"

#ifdef __EMSCRIPTEN__
#include <GLES3/gl3.h>
#include <emscripten/val.h>
#else
#include <GL/glew.h>
#endif

#include "z-engine/include/z-vgengine.h"

namespace {

uint32_t makeSampleChecksum(const std::vector<uint8_t>& pixels) {
    uint32_t checksum{0};
    constexpr size_t kSampleStep{1024};

    for (size_t index{0}; index < pixels.size(); index += kSampleStep) {
        checksum = checksum * 131u + pixels[index];
    }

    return checksum;
}

#ifdef __EMSCRIPTEN__
void showBitmapDebugCanvas(const std::vector<uint8_t>& pixels, const int width, const int height) {
    if (pixels.empty() || width <= 0 || height <= 0) {
        return;
    }

    constexpr int kPreviewScale{10};
    const int previewWidth{std::max(1, width / kPreviewScale)};
    const int previewHeight{std::max(1, height / kPreviewScale)};

    auto document = emscripten::val::global("document");
    auto canvas = document.call<emscripten::val>("getElementById", std::string("bitmap-preview"));

    if (canvas.isNull() || canvas.isUndefined()) {
        canvas = document.call<emscripten::val>("createElement", std::string("canvas"));
        canvas.set("id", std::string("bitmap-preview"));

        auto style = canvas["style"];
        style.set("position", std::string("fixed"));
        style.set("right", std::string("12px"));
        style.set("bottom", std::string("12px"));
        style.set("zIndex", std::string("9999"));
        style.set("border", std::string("1px solid rgba(0, 0, 0, 0.35)"));
        style.set("background", std::string("#fff"));
        style.set("imageRendering", std::string("pixelated"));

        document["body"].call<void>("appendChild", canvas);
    }

    canvas.set("width", width);
    canvas.set("height", height);
    canvas["style"].set("width", std::to_string(previewWidth) + "px");
    canvas["style"].set("height", std::to_string(previewHeight) + "px");

    std::vector<uint8_t> flipped(pixels.size());
    const size_t rowSize{static_cast<size_t>(width) * 4};
    for (int y{0}; y < height; ++y) {
        const size_t sourceOffset{static_cast<size_t>(height - 1 - y) * rowSize};
        const size_t targetOffset{static_cast<size_t>(y) * rowSize};
        std::copy_n(pixels.data() + sourceOffset, rowSize, flipped.data() + targetOffset);
    }

    auto uint8Array = emscripten::val::global("Uint8ClampedArray")
                          .new_(emscripten::typed_memory_view(flipped.size(), flipped.data()));
    auto imageData = emscripten::val::global("ImageData").new_(uint8Array, width, height);
    auto context = canvas.call<emscripten::val>("getContext", std::string("2d"));
    context.call<void>("putImageData", imageData, 0, 0);
}
#endif

}  // namespace

// 构造函数：初始化指针和基础数值
ZWindow::ZWindow() : zEngine(nullptr), zWidth(800), zHeight(600), zDpr(1.0f) {
    init();
}

void ZWindow::init() {
    zSurface = std::make_unique<ZCanvasSurface>(zWidth, zHeight);
    zEngine = new ZVgEngine();

    zDocumentPainter = std::make_shared<ZDocumentPainter>();
    zOverlayPainter = std::make_shared<ZOverlayPainter>(zEditorContext);
}

void ZWindow::draw() {
    {
        // ZBitmapSurface bitmapSurface(zWidth, zHeight);
        // bitmapSurface.makeCurrent();

        zEngine->beginFrame(zCssWidth, zCssHeight, zDpr, zWidth, zHeight);
        zDocumentPainter->draw(zEngine);
        zOverlayPainter->draw(zEngine);

        if (zOverlayDrawer) {
            zOverlayDrawer(zEngine);
        }

        zEngine->endFrame();

        // const auto pixels = bitmapSurface.readPixels();
        // bitmapSurface.destroy();
#ifdef __EMSCRIPTEN__
        // showBitmapDebugCanvas(pixels, zWidth, zHeight);
#endif
    }

    zSurface->makeCurrent();
    zEngine->beginFrame(zCssWidth, zCssHeight, zDpr, zWidth, zHeight);
    zDocumentPainter->draw(zEngine);
    zOverlayPainter->draw(zEngine);

    // 绘制图片.
    if (zOverlayDrawer) {
        zOverlayDrawer(zEngine);
    }
    zEngine->endFrame();
    zSurface->present();
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

void ZWindow::setOverlayDrawer(OverlayDrawer overlayDrawer) {
    zOverlayDrawer = std::move(overlayDrawer);
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
}
