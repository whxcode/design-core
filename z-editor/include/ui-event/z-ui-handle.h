#pragma once

#include <memory>
#include <vector>

#include "z-editor/include/ui-event/z-ui-handle-event.h"

class ZEditorContext;

class ZUIHandle {
public:
    explicit ZUIHandle(ZEditorContext* context);

    void onUIEvent(const ZUIEvent& event);
    void addEffectHandler(std::shared_ptr<ZUIHandleEvent> handleEvent);
    void setActiveHandler(std::shared_ptr<ZUIHandleEvent> handleEvent);

private:
    std::vector<std::shared_ptr<ZUIHandleEvent>> zEffectHandlers;
    std::shared_ptr<ZUIHandleEvent> zActiveHandler{nullptr};
};
