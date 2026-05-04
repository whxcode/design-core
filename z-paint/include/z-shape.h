#pragma once

#include <vector>

#include "z-tools/include/z-guid.h"
#include "z-tools/include/z-type.h"

class IZEngine;
class ZEditorContext;

class ZShape {
public:
    virtual ~ZShape() = default;

    const ZGuid& getId() const;
    void addChild(z_sp<ZShape> child);
    virtual bool getVisible() const;
    virtual void render(IZEngine* engine, ZEditorContext* context);

protected:
    void renderChildren(IZEngine* engine, ZEditorContext* context);

private:
    ZGuid zId{ZGuid::MakeGuid()};
    std::vector<z_sp<ZShape>> zChildren;
};
