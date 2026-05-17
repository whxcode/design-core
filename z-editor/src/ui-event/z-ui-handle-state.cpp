#include "z-editor/include/ui-event/z-ui-handle-state.h"

#include "z-editor/include/z-editor-context.h"

void ZUIHandleState::update(const ZUIEvent& event, const ZEditorContext* context) {
    switch (event.type) {
        case ZUIEventType::zMouseDown:
            zMouseDownPoint = toWorldPoint(event, context);
            zCurrentPoint = zMouseDownPoint;
            zPressDown = true;
            break;
        case ZUIEventType::zMouseMove:
            zCurrentPoint = toWorldPoint(event, context);
            break;
        case ZUIEventType::zMouseUp:
            zMouseUpPoint = toWorldPoint(event, context);
            zCurrentPoint = zMouseUpPoint;
            zPressDown = false;
            break;
        case ZUIEventType::zMouseWheel:
        case ZUIEventType::zKeyDown:
        case ZUIEventType::zKeyUp:
        case ZUIEventType::zUnknown:
            break;
    }
}

ZPoint ZUIHandleState::mouseDownPoint() const {
    return zMouseDownPoint;
}

ZPoint ZUIHandleState::currentPoint() const {
    return zCurrentPoint;
}

ZPoint ZUIHandleState::mouseUpPoint() const {
    return zMouseUpPoint;
}

bool ZUIHandleState::pressDown() const {
    return zPressDown;
}

ZPoint ZUIHandleState::toWorldPoint(const ZUIEvent& event, const ZEditorContext* context) const {
    if (!context) {
        return ZPoint(event.x, event.y);
    }

    const auto viewport = context->getViewportData();
    if (viewport.scale == 0.0f) {
        return ZPoint(event.x, event.y);
    }

    return ZPoint((event.x - viewport.offsetX) / viewport.scale,
                  (event.y - viewport.offsetY) / viewport.scale);
}
