#pragma once

#include <unordered_map>

#include "z-engine/include/z-engine.h"

class NVGcontext;
class IZEngine;

// 如果非要前置声明 SDL_GLContext，标准的做法是直接包含头文件或者使用 void*
// 但最稳妥、最干净的做法是：
typedef void* SDL_GLContext;

class ZVgEngine : public IZEngine {
public:
    ZVgEngine();

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

    void beginFrame(float zWidth, float zHeight, float zDpr, int zPixelWidth,
                    int zPixelHeight) override;
    void endFrame() override;
    // void flush() override;

    void* getContext() override {
        return zVg;
    };

private:
    struct ZImageCacheItem {
        int imageHandle{0};
        size_t size{0};
    };

    // int zWidth{800};
    // int zHeight{800};
    // float zDpr{0};
    // SDL_Window* sWindow{nullptr};
    // SDL_GLContext zGlContext{nullptr};

    NVGcontext* zVg{nullptr};
    std::unordered_map<uintptr_t, ZImageCacheItem> zImageCache;
};
