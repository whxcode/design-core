#pragma once

class SDL_Window;
class SDL_Renderer;
class IZEngine;

class ZWindow {
public:
    ZWindow();

public:
    void draw();
    void setTitle();

private:
    SDL_Window* sWindow{nullptr};
    SDL_Renderer* sRender{nullptr};
    IZEngine* zEngine{nullptr};
};
