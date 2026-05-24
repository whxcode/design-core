#pragma once

#include "z-window/include/z-surface.h"

struct SDL_Window;
using SDL_GLContext = void*;

class ZCanvasSurface : public ZSurface {
public:
    ZCanvasSurface(int pixelWidth, int pixelHeight);
    ~ZCanvasSurface() override;

    void makeCurrent() override;
    void present() override;

private:
    SDL_Window* zWindow{nullptr};
    SDL_GLContext zContext{nullptr};
};
