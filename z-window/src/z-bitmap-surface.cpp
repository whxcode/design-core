#include "z-window/include/z-bitmap-surface.h"

#ifdef __EMSCRIPTEN__
#include <GLES3/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <cstdio>

ZBitmapSurface::ZBitmapSurface(const int pixelWidth, const int pixelHeight)
    : zPixelWidth(pixelWidth), zPixelHeight(pixelHeight) {
    glGenFramebuffers(1, &zFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, zFramebuffer);

    glGenTextures(1, &zTexture);
    glBindTexture(GL_TEXTURE_2D, zTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, zPixelWidth, zPixelHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, zTexture, 0);

    glGenRenderbuffers(1, &zStencilBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, zStencilBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX8, zPixelWidth, zPixelHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER,
                              zStencilBuffer);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        printf("ZBitmapSurface framebuffer is not complete\n");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

ZBitmapSurface::~ZBitmapSurface() {
    destroy();
}

void ZBitmapSurface::makeCurrent() {
    glBindFramebuffer(GL_FRAMEBUFFER, zFramebuffer);
}

void ZBitmapSurface::present() {
}

std::vector<uint8_t> ZBitmapSurface::readPixels() {
    std::vector<uint8_t> pixels(static_cast<size_t>(zPixelWidth) *
                                static_cast<size_t>(zPixelHeight) * 4);

    glReadPixels(0, 0, zPixelWidth, zPixelHeight, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());

    return pixels;
}

void ZBitmapSurface::destroy() {
    if (zStencilBuffer != 0) {
        glDeleteRenderbuffers(1, &zStencilBuffer);
        zStencilBuffer = 0;
    }

    if (zTexture != 0) {
        glDeleteTextures(1, &zTexture);
        zTexture = 0;
    }

    if (zFramebuffer != 0) {
        glDeleteFramebuffers(1, &zFramebuffer);
        zFramebuffer = 0;
    }
}
