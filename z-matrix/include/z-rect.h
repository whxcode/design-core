#pragma once

#include <algorithm>

#include "z-matrix/include/z-point.h"

class ZRect {
public:
    ZRect() = default;
    ZRect(float left, float top, float right, float bottom);

    static ZRect MakeXYWH(float x, float y, float width, float height);
    static ZRect MakeEmpty();

    bool isEmpty() const;
    float left() const;
    float top() const;
    float right() const;
    float bottom() const;
    float width() const;
    float height() const;

    bool contains(const ZPoint& point) const;
    void join(const ZRect& rect);

private:
    float zLeft{0.0f};
    float zTop{0.0f};
    float zRight{0.0f};
    float zBottom{0.0f};
};
