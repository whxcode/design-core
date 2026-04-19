#include "z-engine/include/z-vgengine.h"

#define NANOVG_GLES3_IMPLEMENTATION

// 1. 先引入 GL 头文件，定义 GLuint 等类型
#ifdef __EMSCRIPTEN__
#include <GLES3/gl3.h>  // 或者 <GLES2/gl2.h>，取决于你用的版本
#else
#include <GL/glew.h>  // 如果你在原生 Linux/Windows 下
#endif

#include "z-engine/libs/nanovg/nanovg.h"
#include "z-engine/libs/nanovg/nanovg_gl.h"

ZVgEngine::ZVgEngine() {
    zVg = nvgCreateGLES3(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
};

void ZVgEngine::save() {
    nvgSave(zVg);
}

void ZVgEngine::drawRect(float zX, float zY, float zW, float zH, const ZStyle& zStyle) {
    auto* vg = (NVGcontext*)zVg;

    nvgBeginPath(vg);
    nvgRect(vg, zX, zY, zW, zH);

    // 1. 处理填充
    nvgFillColor(vg, nvgRGBA((zStyle.zFillColor >> 16) & 0xFF, (zStyle.zFillColor >> 8) & 0xFF,
                             zStyle.zFillColor & 0xFF, 255));
    nvgFill(vg);

    // 2. 处理边框 (如果有的话)
    if (zStyle.zStrokeWidth > 0) {
        // nvgStrokeWidth(vg, zStyle.zStrokeWidth);
        // nvgStrokeColor(vg, nvgRGBA(...));  // 同样逻辑转换
        // nvgStroke(vg);
    }
};

void ZVgEngine::transform() {
}

void ZVgEngine::restore() {
    nvgRestore(zVg);
}

void ZVgEngine::beginFrame(float zWidth, float zHeight, float zDpr) {
    // NanoVG 的标准入口
    nvgBeginFrame(zVg, zWidth, zHeight, zDpr);
}

void ZVgEngine::endFrame() {
    // 强制 GPU 提交渲染指令
    nvgEndFrame(zVg);
}
