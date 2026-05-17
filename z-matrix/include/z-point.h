#pragma once

#include <cmath>
#include <memory>

class ZPoint {
public:
    static ZPoint Make(const float x, const float y);
    static float Distance(const ZPoint& p1, const ZPoint& p2);

public:
    ZPoint() = default;
    ZPoint(const float x, const float y);

    void set(const float x, const float y);
    void offset(const float x, const float y);

    const float x() const;

    const float y() const;

    bool operator==(const ZPoint& other) const;
    bool operator!=(const ZPoint& other) const;

    // 常用数学运算
    void scale(const float scale);
    void negate();

public:
    float zX{0};
    float zY{0};
};
