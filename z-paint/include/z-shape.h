#pragma once

#include <memory>
#include <vector>

class IZEngine;
class ZEditorContext;

class ZShape {
public:
    virtual ~ZShape() = default;

    void addChild(std::shared_ptr<ZShape> child);
    virtual void render(IZEngine* engine, ZEditorContext* context);

protected:
    void renderChildren(IZEngine* engine, ZEditorContext* context);

private:
    std::vector<std::shared_ptr<ZShape>> zChildren;
};
