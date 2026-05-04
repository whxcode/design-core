#include "z-matrix/include/z-rect.h"

ZRect::ZRect(const float left, const float top, const float right, const float bottom)
    : zLeft(left), zTop(top), zRight(right), zBottom(bottom) {
}

ZRect ZRect::MakeXYWH(const float x, const float y, const float width, const float height) {
    return ZRect(x, y, x + width, y + height);
}

ZRect ZRect::MakeLTRB(const float left, const float top, const float right, const float bottom) {
    return ZRect(left, top, right, bottom).normalized();
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

bool ZRect::intersects(const ZRect& rect) const {
    if (isEmpty() || rect.isEmpty()) {
        return false;
    }

    return zLeft <= rect.zRight && zRight >= rect.zLeft && zTop <= rect.zBottom &&
           zBottom >= rect.zTop;
}

ZRect ZRect::normalized() const {
    return ZRect(
        std::min(zLeft, zRight),
        std::min(zTop, zBottom),
        std::max(zLeft, zRight),
        std::max(zTop, zBottom)
    );
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
