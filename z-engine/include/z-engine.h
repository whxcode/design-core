#pragma once
#include <cstddef>
#include <cstdint>

#include "z-matrix/include/z-matrix.h"
#include "z-matrix/include/z-rect.h"

struct ZStyle {
    uint32_t zFillColor{0x000000FF};
    uint32_t zStrokeColor{0x000000FF};
    float zFillAlpha{1.0f};
    float zStrokeWidth{0};
    // 后期扩展：zGradient, zShadow...
};

class IZEngine {
public:
    virtual void drawRect(float zX, float zY, float zW, float zH, const ZStyle& zStyle) = 0;
    virtual void drawRect(float zW, float zH, const ZStyle& zStyle) = 0;
    virtual void drawRect(const ZRect& rect, const ZStyle& zStyle) = 0;
    virtual void drawImage(const uint8_t* bytes, size_t size, float zX, float zY, float zW,
                           float zH) = 0;

public:
    virtual void save() = 0;
    virtual void transform(const ZMatrix& matrix) = 0;
    virtual void restore() = 0;

    virtual void beginFrame(float zWidth, float zHeight, float zDpr) = 0;
    virtual void endFrame() = 0;
    virtual void flush() = 0;

    virtual void* getContext() = 0;
};
