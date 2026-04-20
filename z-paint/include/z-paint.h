#pragma once

#include "z-tools/include/z-type.h"

class IZEngine;
class ZLayerBase;

class ZPaint {
public:
    ZPaint(IZEngine* engine);

public:
    void draw();

    void setComponent(const z_sp<ZLayerBase>& comp);

private:
    IZEngine* zEngine{nullptr};
    z_sp<ZLayerBase> fComponent{nullptr};
};
