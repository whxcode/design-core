#pragma once

#include "z-tools/include/z-type.h"

class IZEngine;
class ZPaint;
class ZLayerBase;

class ZWindow {
public:
    ZWindow();
    void draw();
    void setTitle();
    void setComponent(const z_sp<ZLayerBase>& comp);
    void dump() const;

private:
    void init();

private:
    int zWidth{800};
    int zHeight{800};
    float zDpr{0};

    IZEngine* zEngine{nullptr};
    ZPaint* zPaint{nullptr};
};
