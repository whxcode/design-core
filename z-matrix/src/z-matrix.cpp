#include "z-matrix/include/z-matrix.h"

#include <cmath>
#include <cstring>

/**
 * 矩阵存储顺序说明 (fMat[9]):
 * [ 0:scaleX,  1:skewX,   2:transX ]
 * [ 3:skewY,   4:scaleY,  5:transY ]
 * [ 6:0,       7:0,       8:1      ]
 */

void ZMatrix::setIdentity() {
    fMat[0] = 1.0f;
    fMat[1] = 0.0f;
    fMat[2] = 0.0f;
    fMat[3] = 0.0f;
    fMat[4] = 1.0f;
    fMat[5] = 0.0f;
    fMat[6] = 0.0f;
    fMat[7] = 0.0f;
    fMat[8] = 1.0f;
}

void ZMatrix::setTranslate(float dx, float dy) {
    fMat[0] = 1.0f;
    fMat[1] = 0.0f;
    fMat[2] = dx;
    fMat[3] = 0.0f;
    fMat[4] = 1.0f;
    fMat[5] = dy;
    fMat[6] = 0.0f;
    fMat[7] = 0.0f;
    fMat[8] = 1.0f;
}

void ZMatrix::setScale(float sx, float sy) {
    fMat[0] = sx;
    fMat[1] = 0.0f;
    fMat[2] = 0.0f;
    fMat[3] = 0.0f;
    fMat[4] = sy;
    fMat[5] = 0.0f;
    fMat[6] = 0.0f;
    fMat[7] = 0.0f;
    fMat[8] = 1.0f;
}

void ZMatrix::setRotate(float degrees, float px, float py) {
    // 将角度转换为弧度
    float rad = degrees * 3.1415926535f / 180.0f;
    float s = std::sin(rad);
    float c = std::cos(rad);

    // 绕任意点 (px, py) 旋转的组合矩阵公式：
    // | c  -s  px-c*px+s*py |
    // | s   c  py-s*px-c*py |
    // | 0   0       1       |
    fMat[0] = c;
    fMat[1] = -s;
    fMat[2] = px - c * px + s * py;
    fMat[3] = s;
    fMat[4] = c;
    fMat[5] = py - s * px - c * py;
    fMat[6] = 0.0f;
    fMat[7] = 0.0f;
    fMat[8] = 1.0f;
}

void ZMatrix::setConcat(const ZMatrix& a, const ZMatrix& b) {
    float res[9];

    // 标准 3x3 矩阵乘法: res = a * b
    res[0] = a.fMat[0] * b.fMat[0] + a.fMat[1] * b.fMat[3] + a.fMat[2] * b.fMat[6];
    res[1] = a.fMat[0] * b.fMat[1] + a.fMat[1] * b.fMat[4] + a.fMat[2] * b.fMat[7];
    res[2] = a.fMat[0] * b.fMat[2] + a.fMat[1] * b.fMat[5] + a.fMat[2] * b.fMat[8];

    res[3] = a.fMat[3] * b.fMat[0] + a.fMat[4] * b.fMat[3] + a.fMat[5] * b.fMat[6];
    res[4] = a.fMat[3] * b.fMat[1] + a.fMat[4] * b.fMat[4] + a.fMat[5] * b.fMat[7];
    res[5] = a.fMat[3] * b.fMat[2] + a.fMat[4] * b.fMat[5] + a.fMat[5] * b.fMat[8];

    res[6] = a.fMat[6] * b.fMat[0] + a.fMat[7] * b.fMat[3] + a.fMat[8] * b.fMat[6];
    res[7] = a.fMat[6] * b.fMat[1] + a.fMat[7] * b.fMat[4] + a.fMat[8] * b.fMat[7];
    res[8] = a.fMat[6] * b.fMat[2] + a.fMat[7] * b.fMat[5] + a.fMat[8] * b.fMat[8];

    // 将结果拷贝回 fMat
    std::memcpy(fMat, res, sizeof(res));
}
