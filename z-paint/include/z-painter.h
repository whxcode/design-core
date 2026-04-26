#pragma once

class IZEngine;

class ZPainter {
public:
    virtual ~ZPainter() = default;

    virtual void draw(IZEngine* engine) = 0;
};
