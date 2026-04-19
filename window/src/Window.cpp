#include "window/include/Window.h"

#include <SDL2/SDL.h>

#include <iostream>
#include <thread>

#ifdef __EMSCRIPTEN__
#include <emscripten/html5.h>
#endif

void ZWindow::draw() {
    // 1. 初始化 SDL 视频子系统
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return;
    }

    int width{0};
    int height{0};

#ifdef __EMSCRIPTEN__
    // 获取前端 canvas 的真实宽高
    emscripten_get_canvas_element_size("#canvas", &width, &height);
    printf("size[%d][%d]\n", width, height);
#endif

    sWindow = SDL_CreateWindow("Design Core", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                               width, height, SDL_WINDOW_SHOWN);

    if (!sWindow) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return;
    }
    // 3. 创建渲染器 (赋值给类的成员，不要用 auto)
    sRender = SDL_CreateRenderer(sWindow, -1, SDL_RENDERER_ACCELERATED);

    // 设置画笔颜色：红(255), 绿(0), 蓝(0), 透明度(255)
    SDL_SetRenderDrawColor(sRender, 255, 0, 0, 255);

    // 用上面设置的红色清空/填充整个屏幕
    SDL_RenderClear(sRender);

    // 将画好的内容呈现到屏幕 (Canvas) 上
    SDL_RenderPresent(sRender);
}

void ZWindow::setTitle() {
    printf("call setTitle\n");
};
