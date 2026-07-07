#pragma once

#include <cstdint>
#include <vector>

#include "z-window/include/z-surface.h"

class ZBitmapSurface : public ZSurface {
public:
    ZBitmapSurface(int pixelWidth, int pixelHeight);
    ~ZBitmapSurface() override;

public:
    void makeCurrent() override;
    void present() override;
    void resize(int pixelWidth, int pixelHeight) override;

public:
    std::vector<uint8_t> readPixels();

    void destroy();

private:
    int zPixelWidth{0};
    int zPixelHeight{0};
    unsigned int zFramebuffer{0};
    unsigned int zTexture{0};
    unsigned int zStencilBuffer{0};
};
