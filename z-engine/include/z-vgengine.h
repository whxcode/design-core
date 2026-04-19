#pragma once

#include "z-engine/include/z-engine.h"

class NVGcontext;

class ZVgEngine : public IZEngine {
public:
    ZVgEngine();

public:
    void save() override;
    void drawRect(float zX, float zY, float zW, float zH, const ZStyle& zStyle) override;
    void transform() override;
    void restore() override;

    void beginFrame(float zWidth, float zHeight, float zDpr) override;
    void endFrame() override;

    void* getContext() override {
        return zVg;
    };

private:
    NVGcontext* zVg{nullptr};
};
