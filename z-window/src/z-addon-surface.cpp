#include "z-window/include/z-addon-surface.h"

#ifdef __EMSCRIPTEN__
#include <GLES3/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <algorithm>
#include <utility>

ZAddonSurface::ZAddonSurface(const int pixelWidth, const int pixelHeight)
    : ZCanvasSurface(pixelWidth, pixelHeight), zPixelWidth(pixelWidth), zPixelHeight(pixelHeight) {
}

void ZAddonSurface::present() {
    if (zOnFrame) {
        std::vector<uint8_t> pixels(static_cast<size_t>(zPixelWidth) *
                                    static_cast<size_t>(zPixelHeight) * 4);
        glReadPixels(0, 0, zPixelWidth, zPixelHeight, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());

        std::vector<uint8_t> flipped(pixels.size());
        const size_t rowSize{static_cast<size_t>(zPixelWidth) * 4};
        for (int y{0}; y < zPixelHeight; ++y) {
            const size_t sourceOffset{static_cast<size_t>(zPixelHeight - 1 - y) * rowSize};
            const size_t targetOffset{static_cast<size_t>(y) * rowSize};
            std::copy_n(pixels.data() + sourceOffset, rowSize, flipped.data() + targetOffset);
        }

        zOnFrame(zPixelWidth, zPixelHeight, flipped);
    }

    ZCanvasSurface::present();
}

void ZAddonSurface::setOnFrame(OnFrameCallback callback) {
    zOnFrame = std::move(callback);
}

void ZAddonSurface::resize(const int pixelWidth, const int pixelHeight) {
    if (pixelWidth <= 0 || pixelHeight <= 0) {
        return;
    }

    ZCanvasSurface::resize(pixelWidth, pixelHeight);
    zPixelWidth = pixelWidth;
    zPixelHeight = pixelHeight;
}
