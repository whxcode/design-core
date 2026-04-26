#include "z-editor/include/ui-event/z-ui-handle-event.h"

ZUIHandleEvent::ZUIHandleEvent(ZEditorContext* context) : zContext(context) {
}

bool ZUIHandleEvent::onUIEvent(const ZUIEvent& event) {
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
