#pragma once
#include <cstdint>

#include "z-matrix/include/z-matrix.h"

struct ZStyle {
    uint32_t zFillColor = 0xFFFFFFFF;
    uint32_t zStrokeColor = 0x000000FF;
    float zStrokeWidth = 1.0f;
    // 后期扩展：zGradient, zShadow...
};

class IZEngine {
public:
    virtual void drawRect(float zX, float zY, float zW, float zH, const ZStyle& zStyle) = 0;
    virtual void drawRect(float zW, float zH, const ZStyle& zStyle) = 0;

public:
    virtual void save() = 0;
    virtual void transform(const ZMatrix& matrix) = 0;
    virtual void restore() = 0;

    virtual void beginFrame(float zWidth, float zHeight, float zDpr) = 0;
    virtual void endFrame() = 0;
    virtual void flush() = 0;

    virtual void* getContext() = 0;
};
