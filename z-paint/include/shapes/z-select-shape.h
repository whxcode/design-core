#pragma once

#include "z-matrix/include/z-rect.h"
#include "z-paint/include/z-shape.h"

class ZSelectShape : public ZShape {
public:
    void render(IZEngine* engine, ZEditorContext* context) override;
};
