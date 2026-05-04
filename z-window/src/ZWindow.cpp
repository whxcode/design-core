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

#ifdef __EMSCRIPTEN__
#include <GLES3/gl3.h>
#include <emscripten/html5.h>
#else
#include <GL/glew.h>
#endif

#include "z-engine/include/z-vgengine.h"

// 构造函数：初始化指针和基础数值
ZWindow::ZWindow() : zEngine(nullptr), zWidth(800), zHeight(600), zDpr(1.0f) {
    init();
}

void ZWindow::init() {
    // 2. 同步尺寸
#ifdef __EMSCRIPTEN__
    zDpr = emscripten_get_device_pixel_ratio();
    emscripten_get_canvas_element_size("#canvas", &zWidth, &zHeight);
    double cssWidth = 0.0;
    double cssHeight = 0.0;
    emscripten_get_element_css_size("#canvas", &cssWidth, &cssHeight);
    zCssWidth = cssWidth > 0.0 ? static_cast<float>(cssWidth) : static_cast<float>(zWidth) / zDpr;
    zCssHeight =
        cssHeight > 0.0 ? static_cast<float>(cssHeight) : static_cast<float>(zHeight) / zDpr;
#else
    zCssWidth = static_cast<float>(zWidth);
    zCssHeight = static_cast<float>(zHeight);
#endif

    zEngine = new ZVgEngine(static_cast<int>(zCssWidth), static_cast<int>(zCssHeight), zDpr);
    zDocumentPainter = std::make_shared<ZDocumentPainter>();
    zOverlayPainter = std::make_shared<ZOverlayPainter>(zEditorContext);
}

void ZWindow::draw() {
// Emscripten 环境下，使用这个函数代替 while(true)
#ifdef __EMSCRIPTEN__
    // 0 表示使用浏览器默认的 60FPS
    // true 表示模拟无限循环
    // emscripten_set_main_loop_arg(main_loop_callback, this, 0, true);
#else
    // 原生环境（Windows/Linux）可以保留类似的循环
    while (true) {
        render_frame();
        SDL_Delay(16);  // 约 60 FPS
    }
#endif

    // printf("[%d],[%d]\n", zWidth, zHeight);

    zEngine->beginFrame(zCssWidth, zCssHeight, zDpr);
    zDocumentPainter->draw(zEngine);
    zOverlayPainter->draw(zEngine);

    // 绘制图片.
    if (zOverlayDrawer) {
        zOverlayDrawer(zEngine);
    }
    zEngine->endFrame();
    zEngine->flush();
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

void ZWindow::setTitle() {
    printf("call setTitle\n");
}
