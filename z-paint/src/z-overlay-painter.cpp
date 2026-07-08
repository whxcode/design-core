#include "z-paint/include/z-overlay-painter.h"

#include "include/core/SkCanvas.h"
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

void ZOverlayPainter::draw(SkCanvas* canvas) {
    if (zTrace) {
        zTrace->render(canvas, zContext);
        return;
    }

    if (!zRoot) {
        return;
    }

    zRoot->render(canvas, zContext);
}
