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

void ZShape::render(IZEngine* engine, ZEditorContext* context) {
    renderChildren(engine, context);
}

void ZShape::renderChildren(IZEngine* engine, ZEditorContext* context) {
    for (const auto& child : zChildren) {
        if (child && child->getVisible()) {
            child->render(engine, context);
        }
    }
}
