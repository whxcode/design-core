#pragma once

#include "z-matrix/include/z-rect.h"
#include "z-paint/include/z-shape.h"

class ZSelectShape : public ZShape {
public:
    void setRect(const ZRect& rect);
    bool getVisible() const override;
    void render(IZEngine* engine, ZEditorContext* context) override;

private:
    ZRect zRect{};
};
