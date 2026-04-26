#include "z-editor/include/ui-event/z-ui-handle-event.h"

void ZUIHandleEvent::onUIEvent(const ZUIEvent& event) {
    switch (event.type) {
        case ZUIEventType::zMouseDown:
            onMouseDown(event);
            break;
        case ZUIEventType::zMouseMove:
            onMouseMove(event);
            break;
        case ZUIEventType::zMouseUp:
            onMouseUp(event);
            break;
        case ZUIEventType::zMouseWheel:
            onMouseWheel(event);
            break;
        case ZUIEventType::zKeyDown:
            onKeyDown(event);
            break;
        case ZUIEventType::zKeyUp:
            onKeyUp(event);
            break;
        case ZUIEventType::zUnknown:
            break;
    }
}
