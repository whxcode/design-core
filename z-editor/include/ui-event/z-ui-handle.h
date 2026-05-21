#pragma once

#include <memory>
#include <vector>

#include "z-editor/include/ui-event/z-ui-handle-event.h"
#include "z-editor/include/ui-event/z-ui-handle-state.h"
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
    void switchDrawPathHandler(ZDrawLayerType type = ZDrawLayerType::zRectangle);
    void switchVectorEditorHandler();
    void switchSelectFrameHandler();
    void switchDragHandler();

private:
    ZUIHandleState zState{};
    std::vector<std::shared_ptr<ZUIHandleEvent>> zEffectHandlers;
    std::shared_ptr<ZUIHandleEvent> zActiveHandler{nullptr};
    ZEditorContext* zContext{nullptr};
};
