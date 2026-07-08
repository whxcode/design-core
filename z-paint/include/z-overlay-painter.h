#pragma once

#include "z-paint/include/z-painter.h"
#include "z-tools/include/z-type.h"

class ZEditorContext;
class ZShape;
class ZTrace;
class SkCanvas;

class ZOverlayPainter : public ZPainter {
public:
    explicit ZOverlayPainter(ZEditorContext* context);

    void setRoot(z_sp<ZShape> root);
    void setTrace(ZTrace* trace);
    void draw(SkCanvas* canvas) override;

private:
    ZEditorContext* zContext{nullptr};
    z_sp<ZShape> zRoot{nullptr};
    ZTrace* zTrace{nullptr};
};
