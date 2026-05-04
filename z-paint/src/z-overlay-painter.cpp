#include "z-paint/include/z-overlay-painter.h"

#include <utility>

#include "z-paint/include/z-shape.h"
#include "z-paint/include/z-trace.h"

ZOverlayPainter::ZOverlayPainter(ZEditorContext* context) : zContext(context) {
}

void ZOverlayPainter::setRoot(z_sp<ZShape> root) {
    zRoot = std::move(root);
}

void ZOverlayPainter::setTrace(ZTrace* trace) {
    zTrace = trace;
}

void ZOverlayPainter::draw(IZEngine* engine) {
    if (zTrace) {
        zTrace->render(engine, zContext);
        return;
    }

    if (!zRoot) {
        return;
    }

    zRoot->render(engine, zContext);
}
