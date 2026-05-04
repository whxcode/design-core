#include "z-editor/include/ui-event/z-ui-handle.h"

#include <utility>

#include "z-editor/include/handlers/z-common-handle.h"
#include "z-editor/include/handlers/z-draw-layer-handle.h"
#include "z-editor/include/handlers/z-viewport-handler.h"
#include "z-editor/include/z-editor-context.h"
#include "z-app/include/ZAppEvent.h"

ZUIHandle::ZUIHandle(ZEditorContext* context) : zContext(context) {
    addEffectHandler(std::make_shared<ZViewportHandler>(ZHandlerType::zViewport, context));
    setActiveHandler(std::make_shared<ZUICommonHandle>(ZHandlerType::zCommon, context));
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
    if (zContext && zContext->getAppEvent()) {
        zContext->getAppEvent()->emit(ZAppEventType::zHandlerChanged);
    }
}

void ZUIHandle::switchHandler(const ZHandlerType type) {
    switch (type) {
        case ZHandlerType::zCommon:
            switchCommonHandler();
            return;
        case ZHandlerType::zDrawLayer:
            switchDrawPathHandler();
            return;
        case ZHandlerType::zViewport:
            return;
    }
}

ZHandlerType ZUIHandle::activeHandlerType() const {
    if (!zActiveHandler) {
        return ZHandlerType::zCommon;
    }

    return zActiveHandler->zType;
}

void ZUIHandle::switchCommonHandler() {
    setActiveHandler(std::make_shared<ZUICommonHandle>(ZHandlerType::zCommon, zContext));
    printf("ZUIHandle::switchCommonHandler\n");
}

void ZUIHandle::switchDrawPathHandler() {
    setActiveHandler(std::make_shared<ZDrawLayerHandle>(ZHandlerType::zDrawLayer, zContext));
    printf("ZUIHandle::switchDrawPathHandler\n");
}
