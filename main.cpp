#include <SDL2/SDL.h>
#include <emscripten.h>
#include <emscripten/bind.h>

#include <iostream>
#include <thread>

#include "app/include/App.h"
#include "document/include/Document.h"
#include "window/include/Window.h"

extern "C" void force_link_webapi();

// 全局状态，用于演示 JS 修改 C++ 数据
float g_rect_x = 0.0f;

void updatePosition(float x) {
    std::thread{[x]() {
        // 换成朴实的打印，避免复杂的线程 ID 流操作
        printf("updatePosition [%d]: %f\n", std::this_thread::get_id(), x);
    }};
}

void computer(float x) {
    std::thread{[x]() {
        // 换成朴实的打印，避免复杂的线程 ID 流操作
        printf("computer [%d]: %f\n", std::this_thread::get_id(), x);
    }}.detach();
}

// 修改后的渲染循环函数
void main_loop(void* arg) {
    SDL_Renderer* renderer = (SDL_Renderer*)arg;

    // --- 必须有：消耗事件队列，防止死锁 ---
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        // 暂时不需要处理逻辑，但必须 Poll 出来
    }

    // 渲染逻辑
    SDL_SetRenderDrawColor(renderer, 33, 33, 33, 255);
    SDL_RenderClear(renderer);

    SDL_Rect rect = {(int)g_rect_x, 250, 100, 100};
    SDL_SetRenderDrawColor(renderer, 0, 200, 255, 255);
    SDL_RenderFillRect(renderer, &rect);

    SDL_RenderPresent(renderer);
}

int main() {
    force_link_webapi();  // 强制链接器加载 webapi 模块
    ZApp::Get().startup();
    return 0;
}
