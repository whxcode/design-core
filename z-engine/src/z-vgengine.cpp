#pragma once
#include "z-engine/include/z-vgengine.h"

#define NANOVG_GLES3_IMPLEMENTATION

// 1. 先引入 GL 头文件，定义 GLuint 等类型
#ifdef __EMSCRIPTEN__
#include <GLES3/gl3.h>  // 或者 <GLES2/gl2.h>，取决于你用的版本
#else
#include <GL/glew.h>  // 如果你在原生 Linux/Windows 下
#endif

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>  // 建议加上，确保 GL 宏定义完整
#include <SDL2/SDL_video.h>   // 必须加上这个，它定义了 GLContext 和相关的操作函数

#include <algorithm>

#include "z-engine/libs/nanovg/nanovg.h"
#include "z-engine/libs/nanovg/nanovg_gl.h"
#include "z-tools/include/z-editor-theme.h"

namespace {

void applyFillAndStroke(NVGcontext* vg, const ZStyle& zStyle) {
    const auto fillAlpha =
        static_cast<unsigned char>(std::clamp(zStyle.zFillAlpha, 0.0f, 1.0f) * 255.0f);
    nvgFillColor(vg, nvgRGBA((zStyle.zFillColor >> 16) & 0xFF, (zStyle.zFillColor >> 8) & 0xFF,
                             zStyle.zFillColor & 0xFF, fillAlpha));
    nvgFill(vg);

    if (zStyle.zStrokeWidth > 0) {
        nvgStrokeColor(
            vg, nvgRGBA((zStyle.zStrokeColor >> 16) & 0xFF, (zStyle.zStrokeColor >> 8) & 0xFF,
                        zStyle.zStrokeColor & 0xFF, 255));
        nvgStrokeWidth(vg, zStyle.zStrokeWidth);
        nvgStroke(vg);
    }
}

}  // namespace

ZVgEngine::ZVgEngine(int w, int h, float dpr) : zWidth(w), zHeight(h), zDpr(dpr) {
    init();
};

void ZVgEngine::init() {
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

    /*
    printf("--- GPU Hardware Report ---\n");
    printf("zRenderer: %s\n", zRenderer);
    printf("zVendor:   %s\n", zVendor);
    printf("zVersion:  %s\n", zVersion);
    printf("zGLSL:     %s\n", zGlslVersion);
    printf("---------------------------\n");

    printf("size[%d x %d],dpr[%f]\n", zWidth, zHeight, zDpr);
  */

    zVg = nvgCreateGLES3(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
}

void ZVgEngine::drawRect(float zX, float zY, float zW, float zH, const ZStyle& zStyle) {
    auto* vg = (NVGcontext*)zVg;

    nvgBeginPath(vg);
    nvgRect(vg, zX, zY, zW, zH);
    applyFillAndStroke(vg, zStyle);
};

void ZVgEngine::drawRect(float zW, float zH, const ZStyle& zStyle) {
    drawRect(0, 0, zW, zH, zStyle);
};

void ZVgEngine::drawRect(const ZRect& rect, const ZStyle& zStyle) {
    drawRect(rect.left(), rect.top(), rect.width(), rect.height(), zStyle);
};

void ZVgEngine::drawOval(float zW, float zH, const ZStyle& zStyle) {
    auto* vg = (NVGcontext*)zVg;

    nvgBeginPath(vg);
    nvgEllipse(vg, zW * 0.5f, zH * 0.5f, zW * 0.5f, zH * 0.5f);
    applyFillAndStroke(vg, zStyle);
}

void ZVgEngine::drawPath(const ZPathDataArray& paths, const ZWindingRule windingRule,
                         const ZStyle& zStyle) {
    if (paths.empty()) {
        return;
    }

    auto* vg = (NVGcontext*)zVg;

    nvgBeginPath(vg);
    for (const auto& path : paths) {
        if (path.points.empty()) {
            continue;
        }

        const auto& firstPoint = path.points.front();
        nvgMoveTo(vg, firstPoint.point.x(), firstPoint.point.y());

        for (size_t index = 1; index < path.points.size(); ++index) {
            const auto& prevPoint = path.points[index - 1];
            const auto& currentPoint = path.points[index];

            // 曲线控制点按 MkPathPoint 语义处理：
            // prev.curveFrom 是上一点出控制点，current.curveTo 是当前点入控制点。
            if (prevPoint.hasCurveFrom || currentPoint.hasCurveTo) {
                const auto controlFrom =
                    prevPoint.hasCurveFrom ? prevPoint.curveFrom : prevPoint.point;
                const auto controlTo =
                    currentPoint.hasCurveTo ? currentPoint.curveTo : currentPoint.point;
                nvgBezierTo(vg, controlFrom.x(), controlFrom.y(), controlTo.x(), controlTo.y(),
                            currentPoint.point.x(), currentPoint.point.y());
            } else {
                nvgLineTo(vg, currentPoint.point.x(), currentPoint.point.y());
            }
        }

        if (path.isClosed) {
            nvgClosePath(vg);
        }
    }

    // NanoVG 当前封装不直接暴露全局 even-odd fill rule；先保留参数，后续做复合路径时再扩展。
    (void)windingRule;
    applyFillAndStroke(vg, zStyle);
}

void ZVgEngine::drawImage(const uint8_t* bytes, size_t size, float zX, float zY, float zW,
                          float zH) {
    if (!zVg || !bytes || size == 0) {
        return;
    }

    const uintptr_t key = reinterpret_cast<uintptr_t>(bytes);
    auto& cacheItem = zImageCache[key];

    if (cacheItem.imageHandle != 0 && cacheItem.size != size) {
        nvgDeleteImage(zVg, cacheItem.imageHandle);
        cacheItem.imageHandle = 0;
    }

    if (cacheItem.imageHandle == 0) {
        cacheItem.imageHandle =
            nvgCreateImageMem(zVg, 0, const_cast<unsigned char*>(bytes), (int)size);
        cacheItem.size = size;
    }

    const int image = cacheItem.imageHandle;
    if (image <= 0) {
        printf("drawImage: decode failed, size=%zu\n", size);
        return;
    }

    int imageW = 0;
    int imageH = 0;
    nvgImageSize(zVg, image, &imageW, &imageH);

    const float drawW = zW > 0 ? zW : (float)imageW;
    const float drawH = zH > 0 ? zH : (float)imageH;

    nvgBeginPath(zVg);
    nvgRect(zVg, zX, zY, drawW, drawH);
    const NVGpaint paint = nvgImagePattern(zVg, zX, zY, drawW, drawH, 0.0f, image, 1.0f);
    nvgFillPaint(zVg, paint);
    nvgFill(zVg);
}

void ZVgEngine::save() {
    nvgSave(zVg);
}

void ZVgEngine::restore() {
    nvgRestore(zVg);
}

void ZVgEngine::transform(const ZMatrix& matrix) {
    // 基础检查：确保 NanoVG 上下文已经初始化
    if (!zVg) return;

    // 直接透传给 NanoVG
    // 严格对应 NanoVG 的 a, b, c, d, e, f 布局
    nvgTransform(zVg,
                 matrix.get(0),  // a (scaleX)
                 matrix.get(3),  // b (skewY)
                 matrix.get(1),  // c (skewX)
                 matrix.get(4),  // d (scaleY)
                 matrix.get(2),  // e (transX)
                 matrix.get(5)   // f (transY)
    );

    // nvgRestore(zVg);
}

void ZVgEngine::beginFrame(float zWidth, float zHeight, float zDpr, const int zPixelWidth,
                           const int zPixelHeight) {
    glViewport(0, 0, zPixelWidth, zPixelHeight);
    const auto backgroundColor = ZEditorTheme::GetColor(ZEditorThemeToken::zCanvasBackground);
    glClearColor(((backgroundColor >> 16) & 0xFF) / 255.0f,
                 ((backgroundColor >> 8) & 0xFF) / 255.0f,
                 (backgroundColor & 0xFF) / 255.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // NanoVG 的标准入口
    nvgBeginFrame(zVg, zWidth, zHeight, zDpr);
}

void ZVgEngine::endFrame() {
    // 强制 GPU 提交渲染指令
    nvgEndFrame(zVg);
}

void ZVgEngine::flush() {
    SDL_GL_SwapWindow(sWindow);
}
