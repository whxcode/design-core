
#include "z-matrix/include/z-size.h"

ZSize::ZSize() : zWidth(0), zHeight(0) {
}

ZSize::ZSize(float w, float h) : zWidth(w), zHeight(h) {
}

ZSize ZSize::Make(float w, float h) {
    return ZSize(w, h);
}

ZSize ZSize::MakeEmpty() {
    return ZSize(0, 0);
}

void ZSize::set(float w, float h) {
    zWidth = w;
    zHeight = h;
}

bool ZSize::isEmpty() const {
    return zWidth <= 0 || zHeight <= 0;
}

float ZSize::width() const {
    return zWidth;
}

float ZSize::height() const {
    return zHeight;
}

bool ZSize::operator==(const ZSize& other) const {
    return zWidth == other.zWidth && zHeight == other.zHeight;
}

bool ZSize::operator!=(const ZSize& other) const {
    return !(*this == other);
}
