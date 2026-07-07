
#pragma once

class ZSurface {
public:
    virtual ~ZSurface() = default;

    virtual void makeCurrent() = 0;
    virtual void present() = 0;
    virtual void resize(int pixelWidth, int pixelHeight) = 0;
};
