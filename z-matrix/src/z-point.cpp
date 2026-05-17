#include "z-matrix/include/z-point.h"

ZPoint ZPoint::Make(const float x, const float y) {
    return ZPoint(x, y);
}

float ZPoint::Distance(const ZPoint& p1, const ZPoint& p2) {
    return std::sqrt((p2.zX - p1.zX) * (p2.zX - p1.zX) + (p2.zY - p1.zY) * (p2.zY - p1.zY));
};

ZPoint::ZPoint(const float x, const float y) : zX(x), zY(y) {
}
void ZPoint::set(const float x, const float y) {
    zX = x;
    zY = y;
}

void ZPoint::offset(const float dx, const float dy) {
    zX += dx;
    zY += dy;
}

const float ZPoint::x() const {
    return zX;
}

const float ZPoint::y() const {
    return zY;
}

void ZPoint::scale(const float scale) {
    zX *= scale;
    zY *= scale;
}

void ZPoint::negate() {
    zX = -zX;
    zY = -zY;
}

bool ZPoint::operator==(const ZPoint& other) const {
    return zX == other.zX && zY == other.zY;
}

bool ZPoint::operator!=(const ZPoint& other) const {
    return !(*this == other);
}
