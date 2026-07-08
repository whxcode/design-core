#pragma once

#include "z-document/include/layers/z-layerbase.h"
#include "z-paint/include/z-shape.h"

class ZHoverShape : public ZShape {
public:
    explicit ZHoverShape(z_sp<ZLayerBase> layer);

    void setLayer(z_sp<ZLayerBase> layer);
    bool getVisible() const override;
    void render(SkCanvas* canvas, ZEditorContext* context) override;

private:
    z_sp<ZLayerBase> zLayer{nullptr};
};
