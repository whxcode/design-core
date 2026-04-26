#include "z-paint/include/z-shape.h"

#include <utility>

void ZShape::addChild(std::shared_ptr<ZShape> child) {
    zChildren.push_back(std::move(child));
}

void ZShape::render(IZEngine* engine, ZEditorContext* context) {
    renderChildren(engine, context);
}

void ZShape::renderChildren(IZEngine* engine, ZEditorContext* context) {
    for (const auto& child : zChildren) {
        if (child) {
            child->render(engine, context);
        }
    }
}
