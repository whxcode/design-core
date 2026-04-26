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
#endif

    zEngine = new ZVgEngine(zWidth, zHeight, zDpr);
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

    zEngine->save();
    zEngine->beginFrame((float)zWidth / zDpr, (float)zHeight / zDpr, zDpr);
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

void ZWindow::setOverlayRoot(std::shared_ptr<ZShape> root) {
    zOverlayPainter->setRoot(std::move(root));
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
