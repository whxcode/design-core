#include "z-editor/include/ui-event/z-ui-handle-event.h"

#include "z-editor/include/z-editor-context.h"

ZUIHandleEvent::ZUIHandleEvent(const ZHandlerType type, ZEditorContext* context)
    : zType(type), zContext(context) {
}

bool ZUIHandleEvent::onUIEvent(const ZUIEvent& event) {
    switch (event.type) {
        case ZUIEventType::zMouseDown:
            zMouseDownPoint = toWorldPoint(event);
            zCurrentPoint = zMouseDownPoint;
            break;
        case ZUIEventType::zMouseMove:
            zCurrentPoint = toWorldPoint(event);
            break;
        case ZUIEventType::zMouseUp:
            zMouseUpPoint = toWorldPoint(event);
            zCurrentPoint = zMouseUpPoint;
            break;
        case ZUIEventType::zMouseWheel:
        case ZUIEventType::zKeyDown:
        case ZUIEventType::zKeyUp:
        case ZUIEventType::zUnknown:
            break;
    }

    switch (event.type) {
        case ZUIEventType::zMouseDown:
            return onMouseDown(event);
        case ZUIEventType::zMouseMove:
            return onMouseMove(event);
        case ZUIEventType::zMouseUp:
            return onMouseUp(event);
        case ZUIEventType::zMouseWheel:
            return onMouseWheel(event);
        case ZUIEventType::zKeyDown:
            return onKeyDown(event);
        case ZUIEventType::zKeyUp:
            return onKeyUp(event);
        case ZUIEventType::zUnknown:
            break;
    }

    return false;
}

bool ZUIHandleEvent::onMouseDown(const ZUIEvent&) {
    return false;
}

bool ZUIHandleEvent::onMouseMove(const ZUIEvent&) {
    return false;
}

bool ZUIHandleEvent::onMouseUp(const ZUIEvent&) {
    return false;
}

bool ZUIHandleEvent::onMouseWheel(const ZUIEvent&) {
    return false;
}

bool ZUIHandleEvent::onKeyDown(const ZUIEvent&) {
    return false;
}

bool ZUIHandleEvent::onKeyUp(const ZUIEvent&) {
    return false;
}

ZPoint ZUIHandleEvent::getMouseDownPoint() const {
    return zMouseDownPoint;
}

ZPoint ZUIHandleEvent::getCurrentPoint() const {
    return zCurrentPoint;
}

ZPoint ZUIHandleEvent::getMouseUpPoint() const {
    return zMouseUpPoint;
}

ZPoint ZUIHandleEvent::toWorldPoint(const ZUIEvent& event) const {
    if (!zContext) {
        return ZPoint(event.x, event.y);
    }

    const auto viewport = zContext->getViewportData();
    if (viewport.scale == 0.0f) {
        return ZPoint(event.x, event.y);
    }

    return ZPoint((event.x - viewport.offsetX) / viewport.scale,
                  (event.y - viewport.offsetY) / viewport.scale);
}
