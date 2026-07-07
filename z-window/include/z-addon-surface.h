#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <vector>

#include "z-window/include/z-canvas-surface.h"

class ZAddonSurface : public ZCanvasSurface {
public:
    using OnFrameCallback = std::function<void(int, int, const std::vector<uint8_t>&)>;

    ZAddonSurface(int pixelWidth, int pixelHeight);

    void present() override;
    void resize(int pixelWidth, int pixelHeight) override;
    void setOnFrame(OnFrameCallback callback);

private:
    int zPixelWidth{0};
    int zPixelHeight{0};
    OnFrameCallback zOnFrame{nullptr};
};
