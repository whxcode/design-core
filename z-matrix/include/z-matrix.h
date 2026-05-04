// z-matrix.h
#pragma once
#include <cmath>
#include <cstddef>

class ZPoint;
class ZRect;

class ZMatrix {
public:
    // --- 静态工厂 (保持洋气) ---
    static ZMatrix Translate(float dx, float dy);
    static ZMatrix Identity() {
        return ZMatrix().setIdentity();
    };

public:
    ZMatrix();

    bool operator==(const ZMatrix& other) const;
    bool operator!=(const ZMatrix& other) const;
    ZMatrix operator*(const ZMatrix& other) const;

public:
    // --- 基础设置 (返回引用支持链式) ---
    ZMatrix& setIdentity();

    ZMatrix& setTranslate(float dx, float dy);

    ZMatrix& setScale(float sx, float sy, float px = 0, float py = 0);

    // --- Pre-Transform (M' = M * Other) ---
    // 逻辑：在当前变换之前增加一个操作
    ZMatrix& preTranslate(float dx, float dy);

    ZMatrix& preScale(float sx, float sy, float px = 0, float py = 0);

    ZMatrix& preRotate(float degrees, float px = 0, float py = 0);

    // --- Post-Transform (M' = Other * M) ---
    // 逻辑：在当前变换之后增加一个操作
    ZMatrix& postTranslate(float dx, float dy);

    ZMatrix& postScale(float sx, float sy, float px = 0, float py = 0);

    // --- 核心乘法逻辑 ---
    ZMatrix& preConcat(const ZMatrix& other);

    ZMatrix& postConcat(const ZMatrix& other);

    // 真正的矩阵乘法：this = A * B
    void setConcat(const ZMatrix& a, const ZMatrix& b);

    ZMatrix& setRotate(float degrees, float px = 0, float py = 0);

    ZPoint mapPoint(const ZPoint& point) const;
    ZRect mapRect(const ZRect& rect) const;

    bool invert(ZMatrix* inverse) const;

    inline float get(const size_t i) const {
        return fMat[i];
    }

private:
    float fMat[9];
};
