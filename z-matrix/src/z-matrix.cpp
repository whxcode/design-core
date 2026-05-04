#include "z-matrix/include/z-matrix.h"

#include <algorithm>
#include <cmath>
#include <cstring>

#include "z-matrix/include/z-point.h"
#include "z-matrix/include/z-rect.h"

ZMatrix ZMatrix::Translate(float dx, float dy) {
    return ZMatrix().setTranslate(dx, dy);
}
/**
 * 矩阵存储顺序说明 (fMat[9]):
 * [ 0:scaleX,  1:skewX,   2:transX ]
 * [ 3:skewY,   4:scaleY,  5:transY ]
 * [ 6:0,       7:0,       8:1      ]
 */

ZMatrix::ZMatrix() {
    setIdentity();
}

bool ZMatrix::operator==(const ZMatrix& other) const {
    for (int i = 0; i < 9; ++i) {
        // 考虑到浮点数精度，这里也可以用 fabsf(a-b) < 1e-6f
        if (this->fMat[i] != other.fMat[i]) return false;
    }
    return true;
}

bool ZMatrix::operator!=(const ZMatrix& other) const {
    return !(*this == other);
}

ZMatrix ZMatrix::operator*(const ZMatrix& other) const {
    ZMatrix result;
    result.setConcat(*this, other);
    return result;
}

ZMatrix& ZMatrix::setIdentity() {
    fMat[0] = 1;
    fMat[1] = 0;
    fMat[2] = 0;
    fMat[3] = 0;
    fMat[4] = 1;
    fMat[5] = 0;
    fMat[6] = 0;
    fMat[7] = 0;
    fMat[8] = 1;

    return *this;
}
ZMatrix& ZMatrix::setTranslate(float dx, float dy) {
    setIdentity();
    fMat[2] = dx;
    fMat[5] = dy;
    return *this;
}
ZMatrix& ZMatrix::setScale(float sx, float sy, float px, float py) {
    fMat[0] = sx;
    fMat[1] = 0;
    fMat[2] = px - sx * px;
    fMat[3] = 0;
    fMat[4] = sy;
    fMat[5] = py - sy * py;
    fMat[6] = 0;
    fMat[7] = 0;
    fMat[8] = 1;
    return *this;
}
ZMatrix& ZMatrix::preTranslate(float dx, float dy) {
    ZMatrix m;
    m.setTranslate(dx, dy);
    return this->preConcat(m);
}
ZMatrix& ZMatrix::preScale(float sx, float sy, float px, float py) {
    ZMatrix m;
    m.setScale(sx, sy, px, py);
    return this->preConcat(m);
}
ZMatrix& ZMatrix::preRotate(float degrees, float px, float py) {
    ZMatrix m;
    m.setRotate(degrees, px, py);
    return this->preConcat(m);
}
ZMatrix& ZMatrix::postTranslate(float dx, float dy) {
    ZMatrix m;
    m.setTranslate(dx, dy);
    return this->postConcat(m);
}
ZMatrix& ZMatrix::postScale(float sx, float sy, float px, float py) {
    ZMatrix m;
    m.setScale(sx, sy, px, py);
    return this->postConcat(m);
}
ZMatrix& ZMatrix::preConcat(const ZMatrix& other) {
    ZMatrix tmp;
    tmp.setConcat(*this, other);
    *this = tmp;
    return *this;
}
ZMatrix& ZMatrix::postConcat(const ZMatrix& other) {
    ZMatrix tmp;
    tmp.setConcat(other, *this);
    *this = tmp;
    return *this;
}
void ZMatrix::setConcat(const ZMatrix& a, const ZMatrix& b) {
    float m0 = a.fMat[0] * b.fMat[0] + a.fMat[1] * b.fMat[3];
    float m1 = a.fMat[0] * b.fMat[1] + a.fMat[1] * b.fMat[4];
    float m2 = a.fMat[0] * b.fMat[2] + a.fMat[1] * b.fMat[5] + a.fMat[2];

    float m3 = a.fMat[3] * b.fMat[0] + a.fMat[4] * b.fMat[3];
    float m4 = a.fMat[3] * b.fMat[1] + a.fMat[4] * b.fMat[4];
    float m5 = a.fMat[3] * b.fMat[2] + a.fMat[4] * b.fMat[5] + a.fMat[5];

    fMat[0] = m0;
    fMat[1] = m1;
    fMat[2] = m2;
    fMat[3] = m3;
    fMat[4] = m4;
    fMat[5] = m5;
    fMat[6] = 0;
    fMat[7] = 0;
    fMat[8] = 1;
}
ZMatrix& ZMatrix::setRotate(float degrees, float px, float py) {
    float rad = degrees * 3.14159265f / 180.0f;
    float s = std::sin(rad);
    float c = std::cos(rad);
    fMat[0] = c;
    fMat[1] = -s;
    fMat[2] = px - c * px + s * py;
    fMat[3] = s;
    fMat[4] = c;
    fMat[5] = py - s * px - c * py;
    fMat[6] = 0;
    fMat[7] = 0;
    fMat[8] = 1;
    return *this;
}

ZPoint ZMatrix::mapPoint(const ZPoint& point) const {
    return ZPoint(fMat[0] * point.x() + fMat[1] * point.y() + fMat[2],
                  fMat[3] * point.x() + fMat[4] * point.y() + fMat[5]);
}

ZRect ZMatrix::mapRect(const ZRect& rect) const {
    if (rect.isEmpty()) {
        return ZRect::MakeEmpty();
    }

    const auto p0 = mapPoint(ZPoint(rect.left(), rect.top()));
    const auto p1 = mapPoint(ZPoint(rect.right(), rect.top()));
    const auto p2 = mapPoint(ZPoint(rect.right(), rect.bottom()));
    const auto p3 = mapPoint(ZPoint(rect.left(), rect.bottom()));

    return ZRect(
        std::min({p0.x(), p1.x(), p2.x(), p3.x()}),
        std::min({p0.y(), p1.y(), p2.y(), p3.y()}),
        std::max({p0.x(), p1.x(), p2.x(), p3.x()}),
        std::max({p0.y(), p1.y(), p2.y(), p3.y()})
    );
}

bool ZMatrix::invert(ZMatrix* inverse) const {
    if (!inverse) {
        return false;
    }

    const float det = fMat[0] * fMat[4] - fMat[1] * fMat[3];
    if (std::fabs(det) < 0.000001f) {
        return false;
    }

    const float invDet = 1.0f / det;
    inverse->fMat[0] = fMat[4] * invDet;
    inverse->fMat[1] = -fMat[1] * invDet;
    inverse->fMat[2] = (fMat[1] * fMat[5] - fMat[4] * fMat[2]) * invDet;
    inverse->fMat[3] = -fMat[3] * invDet;
    inverse->fMat[4] = fMat[0] * invDet;
    inverse->fMat[5] = (fMat[3] * fMat[2] - fMat[0] * fMat[5]) * invDet;
    inverse->fMat[6] = 0.0f;
    inverse->fMat[7] = 0.0f;
    inverse->fMat[8] = 1.0f;
    return true;
}
