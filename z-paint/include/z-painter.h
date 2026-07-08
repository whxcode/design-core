#pragma once

class SkCanvas;

class ZPainter {
public:
    virtual ~ZPainter() = default;

    virtual void draw(SkCanvas* canvas) = 0;
};
