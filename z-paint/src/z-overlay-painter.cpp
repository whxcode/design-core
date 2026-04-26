#include "z-paint/include/z-overlay-painter.h"

#include <utility>

#include "z-paint/include/z-shape.h"

ZOverlayPainter::ZOverlayPainter(ZEditorContext* context) : zContext(context) {
}

void ZOverlayPainter::setRoot(std::shared_ptr<ZShape> root) {
    zRoot = std::move(root);
}

void ZOverlayPainter::draw(IZEngine* engine) {
    if (!zRoot) {
        return;
    }

    zRoot->render(engine, zContext);
}
