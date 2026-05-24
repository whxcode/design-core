#include "z-window/include/ZWindow.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>  // 建议加上，确保 GL 宏定义完整
#include <SDL2/SDL_video.h>   // 必须加上这个，它定义了 GLContext 和相关的操作函数

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
    ZBitmapSurface bitmapSurface(zWidth, zHeight);
    bitmapSurface.makeCurrent();

    zEngine->beginFrame(zCssWidth, zCssHeight, zDpr, zWidth, zHeight);
    zDocumentPainter->draw(zEngine);
    zOverlayPainter->draw(zEngine);

    if (zOverlayDrawer) {
        zOverlayDrawer(zEngine);
    }

    zEngine->endFrame();

    const auto pixels = bitmapSurface.readPixels();
    printf("ZBitmapSurface readPixels size[%zu], checksum[%u]\n", pixels.size(),
           makeSampleChecksum(pixels));

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
