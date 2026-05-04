#include "z-matrix/include/z-rect.h"

ZRect::ZRect(const float left, const float top, const float right, const float bottom)
    : zLeft(left), zTop(top), zRight(right), zBottom(bottom) {
}

ZRect ZRect::MakeXYWH(const float x, const float y, const float width, const float height) {
    return ZRect(x, y, x + width, y + height);
}

ZRect ZRect::MakeEmpty() {
    return {};
}

bool ZRect::isEmpty() const {
    return zRight <= zLeft || zBottom <= zTop;
}

float ZRect::left() const {
    return zLeft;
}

float ZRect::top() const {
    return zTop;
}

float ZRect::right() const {
    return zRight;
}

float ZRect::bottom() const {
    return zBottom;
}

float ZRect::width() const {
    return zRight - zLeft;
}

float ZRect::height() const {
    return zBottom - zTop;
}

bool ZRect::contains(const ZPoint& point) const {
    return point.x() >= zLeft && point.x() <= zRight && point.y() >= zTop &&
           point.y() <= zBottom;
}

void ZRect::join(const ZRect& rect) {
    if (rect.isEmpty()) {
        return;
    }

    if (isEmpty()) {
        *this = rect;
        return;
    }

    zLeft = std::min(zLeft, rect.zLeft);
    zTop = std::min(zTop, rect.zTop);
    zRight = std::max(zRight, rect.zRight);
    zBottom = std::max(zBottom, rect.zBottom);
}
