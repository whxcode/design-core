#pragma once

#include <memory>
#include <vector>

#include "z-editor/include/ui-event/z-ui-handle-event.h"
#include "z-matrix/include/z-point.h"

class ZEditorContext;

class ZUIHandle {
public:
    explicit ZUIHandle(ZEditorContext* context);

    void onUIEvent(const ZUIEvent& event);
    void addEffectHandler(std::shared_ptr<ZUIHandleEvent> handleEvent);
    void setActiveHandler(std::shared_ptr<ZUIHandleEvent> handleEvent);
    void switchHandler(ZHandlerType type);
    ZHandlerType activeHandlerType() const;

public:
    void switchCommonHandler();
    void switchDrawPathHandler();
    void switchSelectFrameHandler();
    void switchSelectFrameHandler(const ZPoint& startPoint);
    void switchDragHandler(const ZPoint& startPoint, const ZPoint& currentPoint);

private:
    std::vector<std::shared_ptr<ZUIHandleEvent>> zEffectHandlers;
    std::shared_ptr<ZUIHandleEvent> zActiveHandler{nullptr};
    ZEditorContext* zContext{nullptr};
};
