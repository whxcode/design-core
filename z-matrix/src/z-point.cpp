#include "z-matrix/include/z-point.h"

ZPoint ZPoint::Make(float x, float y) {
    return ZPoint(x, y);
}

void ZPoint::set(float x, float y) {
    zX = x;
    zY = y;
}

void ZPoint::offset(float dx, float dy) {
    zX += dx;
    zY += dy;
}

void ZPoint::scale(float scale) {
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
