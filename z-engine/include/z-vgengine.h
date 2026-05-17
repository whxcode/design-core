#pragma once

#include <unordered_map>

#include "z-engine/include/z-engine.h"

class NVGcontext;
// 前置声明
class SDL_Window;
// class SDL_GLContext; // 暴力删除这一行！它是 typedef，不能这样前置声明
class IZEngine;

// 如果非要前置声明 SDL_GLContext，标准的做法是直接包含头文件或者使用 void*
// 但最稳妥、最干净的做法是：
typedef void* SDL_GLContext;

class ZVgEngine : public IZEngine {
public:
    ZVgEngine(int w, int h, float dpr);

public:
    void init();

public:
    void drawRect(float zX, float zY, float zW, float zH, const ZStyle& zStyle) override;
    void drawRect(float zW, float zH, const ZStyle& zStyle) override;
    void drawRect(const ZRect& rect, const ZStyle& zStyle) override;
    void drawOval(float zW, float zH, const ZStyle& zStyle) override;
    void drawPath(const ZPathDataArray& paths, ZWindingRule windingRule,
                  const ZStyle& zStyle) override;
    void drawImage(const uint8_t* bytes, size_t size, float zX, float zY, float zW,
                   float zH) override;

public:
    void save() override;
    void transform(const ZMatrix& matrix) override;
    void restore() override;

    void beginFrame(float zWidth, float zHeight, float zDpr) override;
    void endFrame() override;
    void flush() override;

    void* getContext() override {
        return zVg;
    };

private:
    struct ZImageCacheItem {
        int imageHandle{0};
        size_t size{0};
    };

    int zWidth{800};
    int zHeight{800};
    float zDpr{0};

    NVGcontext* zVg{nullptr};
    SDL_Window* sWindow{nullptr};
    SDL_GLContext zGlContext{nullptr};
    std::unordered_map<uintptr_t, ZImageCacheItem> zImageCache;
};
