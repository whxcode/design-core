#include "z-editor/include/ui-event/z-ui-handle.h"

#include <utility>

#include "z-editor/include/handlers/z-viewport-handler.h"
#include "z-editor/include/ui-event/z-ui-common-handle.h"

ZUIHandle::ZUIHandle(ZEditorContext* context) {
    addEffectHandler(std::make_shared<ZViewportHandler>(context));
    setActiveHandler(std::make_shared<ZUICommonHandle>(context));
}

void ZUIHandle::onUIEvent(const ZUIEvent& event) {
    for (const auto& handler : zEffectHandlers) {
        if (handler && handler->onUIEvent(event)) {
            return;
        }
    }

    if (zActiveHandler) {
        zActiveHandler->onUIEvent(event);
    }
}

void ZUIHandle::addEffectHandler(std::shared_ptr<ZUIHandleEvent> handleEvent) {
    zEffectHandlers.push_back(std::move(handleEvent));
}

void ZUIHandle::setActiveHandler(std::shared_ptr<ZUIHandleEvent> handleEvent) {
    zActiveHandler = std::move(handleEvent);
}
