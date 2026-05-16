#include "z-editor/include/ui-event/z-ui-handle.h"

#include <utility>

#include "z-editor/include/handlers/z-common-handle.h"
#include "z-editor/include/handlers/z-drag-handler.h"
#include "z-editor/include/handlers/z-draw-layer-handle.h"
#include "z-editor/include/handlers/z-select-frame-handler.h"
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

    const auto activeHandler = zActiveHandler;
    if (activeHandler) {
        activeHandler->onUIEvent(event);
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
        case ZHandlerType::zSelectFrame:
            switchSelectFrameHandler();
            return;
        case ZHandlerType::zDrag:
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

void ZUIHandle::switchDrawPathHandler(const ZDrawLayerType type) {
    setActiveHandler(std::make_shared<ZDrawLayerHandle>(ZHandlerType::zDrawLayer, zContext, type));
    printf("ZUIHandle::switchDrawPathHandler\n");
}

void ZUIHandle::switchSelectFrameHandler() {
    setActiveHandler(std::make_shared<ZSelectFrameHandler>(ZHandlerType::zSelectFrame, zContext));
    printf("ZUIHandle::switchSelectFrameHandler\n");
}

void ZUIHandle::switchSelectFrameHandler(const ZPoint& startPoint) {
    auto handler = std::make_shared<ZSelectFrameHandler>(ZHandlerType::zSelectFrame, zContext);
    handler->zMouseDownPoint = startPoint;
    handler->zCurrentPoint = startPoint;
    setActiveHandler(handler);
    printf("ZUIHandle::switchSelectFrameHandler\n");
}

void ZUIHandle::switchDragHandler(const ZPoint& startPoint, const ZPoint& currentPoint) {
    auto handler = std::make_shared<ZDragHandler>(ZHandlerType::zDrag, zContext);
    handler->zMouseDownPoint = startPoint;
    handler->zCurrentPoint = currentPoint;
    handler->setLastPoint(startPoint);
    setActiveHandler(handler);
    printf("ZUIHandle::switchDragHandler\n");
}
