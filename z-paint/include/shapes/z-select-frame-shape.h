#pragma once

#include "z-matrix/include/z-rect.h"
#include "z-paint/include/z-shape.h"

class ZSelectFrameShape : public ZShape {
public:
    void setRect(const ZRect& rect);
    const ZRect& getRect() const;
    bool getVisible() const override;
    void render(IZEngine* engine, ZEditorContext* context) override;

private:
    ZRect zRect{};
};
