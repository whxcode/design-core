
#pragma once

class ZSurface {
public:
    virtual ~ZSurface() = default;

    virtual void makeCurrent() = 0;
    virtual void present() = 0;
};
