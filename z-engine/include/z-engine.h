#pragma once
#include <cstddef>
#include <cstdint>

#include "z-document/include/models/z-path-data.h"
#include "z-matrix/include/z-matrix.h"
#include "z-matrix/include/z-rect.h"

// 单次绘制属性：一个 ZPaint 对应一次 draw 调用
struct ZStyle {
    uint32_t color{0x000000FF};
    float alpha{1.0f};
    bool isStroke{false};    // true=描边, false=填充
    float strokeWidth{1.0f}; // 仅 isStroke=true 生效
};

class IZEngine {
public:
    virtual void drawRect(float zX, float zY, float zW, float zH, const ZStyle& zStyle) = 0;
    virtual void drawRect(float zW, float zH, const ZStyle& zStyle) = 0;
    virtual void drawRect(const ZRect& rect, const ZStyle& zStyle) = 0;
    virtual void drawOval(float zW, float zH, const ZStyle& zStyle) = 0;
    virtual void drawPath(const ZPathDataArray& paths, ZWindingRule windingRule,
                          const ZStyle& zStyle) = 0;
    virtual void drawImage(const uint8_t* bytes, size_t size, float zX, float zY, float zW,
                           float zH) = 0;

public:
    virtual void save() = 0;
    virtual void transform(const ZMatrix& matrix) = 0;
    virtual void restore() = 0;

    virtual void beginFrame(float zWidth, float zHeight, float zDpr, int zPixelWidth,
                            int zPixelHeight) = 0;
    virtual void endFrame() = 0;

    virtual void* getContext() = 0;
};
