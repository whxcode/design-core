#pragma once

// 前置声明
class SDL_Window;
// class SDL_GLContext; // 暴力删除这一行！它是 typedef，不能这样前置声明
class IZEngine;

// 如果非要前置声明 SDL_GLContext，标准的做法是直接包含头文件或者使用 void*
// 但最稳妥、最干净的做法是：
typedef void* SDL_GLContext;

class ZWindow {
public:
    ZWindow();
    void draw();
    void setTitle();

    void testRender();

private:
    void init();

private:
    int zWidth{800};
    int zHeight{800};
    float zDpr{0};

    SDL_Window* sWindow{nullptr};

    // 注意这里：SDL_GLContext 本身就是指针，不要写成 SDL_GLContext*
    SDL_GLContext zGlContext{nullptr};

    // 如果你已经决定彻底清理 SDL_Renderer，建议把下面这行也删了，保持代码纯净
    // SDL_Renderer* sRender{nullptr};

    IZEngine* zEngine{nullptr};
};
