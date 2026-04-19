#pragma once

class ZPoint {
public:
    ZPoint() : zX(0), zY(0) {
    }

    ZPoint(float x, float y) : zX(x), zY(y) {
    }

    static ZPoint Make(float x, float y);

    void set(float x, float y);
    void offset(float dx, float dy);

    float x() const {
        return zX;
    }
    float y() const {
        return zY;
    }

    bool operator==(const ZPoint& other) const;
    bool operator!=(const ZPoint& other) const;

    // 常用数学运算
    void scale(float scale);
    void negate();

public:
    float zX;
    float zY;
};
