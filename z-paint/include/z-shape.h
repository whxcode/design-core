#pragma once

#include <vector>

#include "z-tools/include/z-guid.h"
#include "z-tools/include/z-type.h"

class SkCanvas;
class ZEditorContext;

class ZShape {
public:
    virtual ~ZShape() = default;

    const ZGuid& getId() const;
    void addChild(z_sp<ZShape> child);
    virtual bool getVisible() const;
    virtual void render(SkCanvas* canvas, ZEditorContext* context);

protected:
    void renderChildren(SkCanvas* canvas, ZEditorContext* context);

private:
    ZGuid zId{ZGuid::MakeGuid()};
    std::vector<z_sp<ZShape>> zChildren;
};
