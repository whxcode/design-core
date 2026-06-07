#pragma once

#include "z-document/include/layers/z-layerbase.h"
#include "z-engine/include/z-engine.h"
#include "z-tools/include/z-type.h"

class IZEngine;
class ZLayerBase;

class ZPainterBase {
public:
    ZPainterBase(IZEngine* engine);

public:
    void draw();
    void setComponent(const z_sp<ZLayerBase>& comp);

private:
    IZEngine* zEngine{nullptr};
    z_sp<ZLayerBase> fComponent{nullptr};
};
