#include "z-window/include/ZWindow.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>  // 建议加上，确保 GL 宏定义完整
#include <SDL2/SDL_video.h>   // 必须加上这个，它定义了 GLContext 和相关的操作函数

#include <iostream>
#include <thread>

#include "z-engine/libs/nanovg/nanovg.h"

#ifdef __EMSCRIPTEN__
#include <GLES3/gl3.h>
#include <emscripten/html5.h>
#else
#include <GL/glew.h>
#endif

#include "z-engine/include/z-vgengine.h"

// 构造函数：初始化指针和基础数值
ZWindow::ZWindow()
    : sWindow(nullptr),
      zGlContext(nullptr),
      zEngine(nullptr),
      zWidth(800),
      zHeight(600),
      zDpr(1.0f) {
}
void ZWindow::init() {
    // 删掉那个 sleep_for，没用的，这是链接库的问题

    SDL_Init(SDL_INIT_VIDEO);

    // 顺序非常关键！
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);

    // 增加这些容错配置
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    // ... 获取宽高的代码 ...

    sWindow =
        SDL_CreateWindow("DesignCore", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, zWidth,
                         zHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);

    zGlContext = SDL_GL_CreateContext(sWindow);

    // 核心动作：必须激活上下文
    SDL_GL_MakeCurrent(sWindow, zGlContext);

    printf("C++: GL Context created. Testing Native GL...\n");

    // 如果 CMake 没加 -s FULL_ES3=1，下面这一行百分之百报错
    GLuint testShader = glCreateShader(GL_VERTEX_SHADER);
    printf("Native GL Test - Shader ID: %u\n", testShader);

    // 在 SDL_GL_MakeCurrent(sWindow, zGlContext); 之后执行

    // 获取渲染器信息
    const GLubyte* zRenderer = glGetString(GL_RENDERER);
    const GLubyte* zVendor = glGetString(GL_VENDOR);
    const GLubyte* zVersion = glGetString(GL_VERSION);
    const GLubyte* zGlslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

    printf("--- GPU Hardware Report ---\n");
    printf("zRenderer: %s\n", zRenderer);
    printf("zVendor:   %s\n", zVendor);
    printf("zVersion:  %s\n", zVersion);
    printf("zGLSL:     %s\n", zGlslVersion);
    printf("---------------------------\n");

    zEngine = new ZVgEngine();
}

// 1. 定义一个全局或静态函数供浏览器调用
void main_loop_callback(void* arg) {
    ZWindow* window = (ZWindow*)arg;
    window->testRender();  // 把你刚才 lambda 里的逻辑搬到这个函数
}

void ZWindow::testRender() {
    if (!zEngine) {
        init();
    }

    // 1. 静态局部变量：只在第一次调用时初始化，不占成员变量位置
    // 增加数据量到 5000 个，配合渐变和混合，足以让 GTX 960 跑起来
    static const int zCount = 5000;
    static float zX[zCount], zY[zCount];
    static bool zIsInitialized = false;

    if (!zIsInitialized) {
        for (int i = 0; i < zCount; i++) {
            zX[i] = (float)(rand() % 1200);
            zY[i] = (float)(rand() % 800);
        }
        zIsInitialized = true;
        printf("C++: Stress test data precomputed internally.\n");
    }

    // 2. 同步尺寸
#ifdef __EMSCRIPTEN__
    zDpr = emscripten_get_device_pixel_ratio();
    emscripten_get_canvas_element_size("#canvas", &zWidth, &zHeight);
#endif

    // 3. 清屏
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // 4. 获取 NanoVG 上下文 (假设 zEngine 提供了 getContext 接口)
    NVGcontext* vg = (NVGcontext*)zEngine->getContext();

    zEngine->beginFrame((float)zWidth, (float)zHeight, zDpr);

    // 5. 渲染循环：均衡 CPU 和 GPU
    for (int i = 0; i < zCount; i++) {
        float x = zX[i];
        float y = zY[i];

        // 增加 GPU 负担：使用线性渐变 + Alpha 混合
        NVGpaint zPaint = nvgLinearGradient(vg, x, y, x + 100, y + 100, nvgRGBA(255, 0, 0, 150),
                                            nvgRGBA(0, 0, 255, 150));

        nvgBeginPath(vg);
        // 使用圆角矩形，增加 GPU 顶点的计算压力
        nvgRoundedRect(vg, x, y, 100, 100, 10.0f);
        nvgFillPaint(vg, zPaint);
        nvgFill(vg);
    }

    zEngine->endFrame();
    SDL_GL_SwapWindow(sWindow);
}

void ZWindow::draw() {
// Emscripten 环境下，使用这个函数代替 while(true)
#ifdef __EMSCRIPTEN__
    // 0 表示使用浏览器默认的 60FPS
    // true 表示模拟无限循环
    emscripten_set_main_loop_arg(main_loop_callback, this, 0, true);
#else
    // 原生环境（Windows/Linux）可以保留类似的循环
    while (true) {
        render_frame();
        SDL_Delay(16);  // 约 60 FPS
    }
#endif
}

void ZWindow::setTitle() {
    printf("call setTitle\n");
}
