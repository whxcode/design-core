#include "z-window/include/z-canvas-surface.h"

#ifdef __EMSCRIPTEN__
#include <GLES3/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>

#include <cstdio>

ZCanvasSurface::ZCanvasSurface(const int pixelWidth, const int pixelHeight) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    auto flags = SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI;
#if defined(Z_ADDON_MODE)
    flags |= SDL_WINDOW_HIDDEN;
#else
    flags |= SDL_WINDOW_SHOWN;
#endif

    zWindow = SDL_CreateWindow("DesignCore", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                               pixelWidth, pixelHeight, flags);
    if (!zWindow) {
        printf("ZCanvasSurface: SDL_CreateWindow failed: %s\n", SDL_GetError());
        return;
    }

    zContext = SDL_GL_CreateContext(zWindow);
    if (!zContext) {
        printf("ZCanvasSurface: SDL_GL_CreateContext failed: %s\n", SDL_GetError());
        return;
    }

    if (SDL_GL_MakeCurrent(zWindow, zContext) != 0) {
        printf("ZCanvasSurface: SDL_GL_MakeCurrent failed: %s\n", SDL_GetError());
        return;
    }

#if !defined(__EMSCRIPTEN__)
    glewExperimental = GL_TRUE;
    const auto glewError = glewInit();
    glGetError();
    if (glewError != GLEW_OK) {
        printf("ZCanvasSurface: glewInit failed: %s\n", glewGetErrorString(glewError));
    }
#endif
}

ZCanvasSurface::~ZCanvasSurface() {
    if (zContext) {
        SDL_GL_DeleteContext(zContext);
        zContext = nullptr;
    }

    if (zWindow) {
        SDL_DestroyWindow(zWindow);
        zWindow = nullptr;
    }
}

void ZCanvasSurface::makeCurrent() {
    SDL_GL_MakeCurrent(zWindow, zContext);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ZCanvasSurface::present() {
    SDL_GL_SwapWindow(zWindow);
}

void ZCanvasSurface::resize(const int pixelWidth, const int pixelHeight) {
    if (!zWindow || pixelWidth <= 0 || pixelHeight <= 0) {
        return;
    }

    SDL_SetWindowSize(zWindow, pixelWidth, pixelHeight);
}
