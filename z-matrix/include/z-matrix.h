#pragma once

class ZMatrix {
public:
    ZMatrix() {
        setIdentity();
    }

    bool operator==(const ZMatrix& other) const;
    bool operator!=(const ZMatrix& other) const;

public:
    void setIdentity();
    void setTranslate(float dx, float dy);
    void setScale(float sx, float sy);
    void setRotate(float degrees, float px = 0, float py = 0);

    // 矩阵乘法：this = A * B
    void setConcat(const ZMatrix& a, const ZMatrix& b);

private:
    float fMat[9];  // 顺序：[scaleX, skewX, transX, skewY, scaleY, transY, 0, 0, 1]
};
