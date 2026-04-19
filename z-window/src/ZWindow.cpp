#include "z-window/include/ZWindow.h"

#include <SDL2/SDL.h>

#include <iostream>
#include <thread>

#include "z-engine/include/z-vgengine.h"

#ifdef __EMSCRIPTEN__
#include <emscripten/html5.h>
#endif

ZWindow::ZWindow() : zEngine(new ZVgEngine()) {
}

void ZWindow::draw() {
    // 1. 变量定义对齐规范
    int zWidth = 0;
    int zHeight = 0;
    float zDpr = 1.0f;

#ifdef __EMSCRIPTEN__
    // 获取设备像素比 (Device Pixel Ratio)，保证渲染清晰度
    zDpr = emscripten_get_device_pixel_ratio();
    // 获取 canvas 逻辑宽高
    emscripten_get_canvas_element_size("#canvas", &zWidth, &zHeight);
#else
    // 原生环境直接拿窗口大小
    SDL_GetWindowSize(zWindow, &zWidth, &zHeight);
#endif
    printf("canvas[%dx%d]\n", zWidth, zHeight);

    // 2. 开启渲染上下文（必须在绘图前）
    // 注意：传入的是逻辑宽高 zWidth/zHeight 和 像素比 zDpr
    zEngine->beginFrame((float)zWidth, (float)zHeight, zDpr);

    // 3. 执行绘图指令
    // 这里是你定义的 z-style 传参方式
    zEngine->drawRect(0.0f, 0.0f, 100.0f, 100.0f, {.zFillColor = 0xFF0000});

    // 4. 暴力收尾（核心：没有这一行绝对不显示！）
    zEngine->endFrame();
}

void ZWindow::setTitle() {
    printf("call setTitle\n");
};
