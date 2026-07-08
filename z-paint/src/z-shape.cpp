#include "z-paint/include/z-shape.h"

#include <utility>

const ZGuid& ZShape::getId() const {
    return zId;
}

void ZShape::addChild(z_sp<ZShape> child) {
    zChildren.push_back(std::move(child));
}

bool ZShape::getVisible() const {
    return true;
}

void ZShape::render(SkCanvas* canvas, ZEditorContext* context) {
    renderChildren(canvas, context);
}

void ZShape::renderChildren(SkCanvas* canvas, ZEditorContext* context) {
    for (const auto& child : zChildren) {
        if (child && child->getVisible()) {
            child->render(canvas, context);
        }
    }
}
