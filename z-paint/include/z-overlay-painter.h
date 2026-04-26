#pragma once

#include <memory>

#include "z-paint/include/z-painter.h"

class ZEditorContext;
class ZShape;

class ZOverlayPainter : public ZPainter {
public:
    explicit ZOverlayPainter(ZEditorContext* context);

    void setRoot(std::shared_ptr<ZShape> root);
    void draw(IZEngine* engine) override;

private:
    ZEditorContext* zContext{nullptr};
    std::shared_ptr<ZShape> zRoot{nullptr};
};
