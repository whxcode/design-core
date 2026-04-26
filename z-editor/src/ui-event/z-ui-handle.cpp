#include "z-editor/include/ui-event/z-ui-handle.h"

#include <iostream>
#include <utility>

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

void ZUICommonHandle::onMouseDown(const ZUIEvent& event) {
    std::cout << "MouseDown: " << event.x << ", " << event.y << std::endl;
}

void ZUICommonHandle::onMouseMove(const ZUIEvent& event) {
    std::cout << "MouseMove: " << event.x << ", " << event.y << std::endl;
}

void ZUICommonHandle::onMouseUp(const ZUIEvent& event) {
    std::cout << "MouseUp: " << event.x << ", " << event.y << std::endl;
}

void ZUICommonHandle::onMouseWheel(const ZUIEvent& event) {
    std::cout << "MouseWheel: " << event.deltaX << ", " << event.deltaY << std::endl;
}

void ZUICommonHandle::onKeyDown(const ZUIEvent& event) {
    std::cout << "KeyDown: " << static_cast<int>(event.keyCode) << std::endl;
}

void ZUICommonHandle::onKeyUp(const ZUIEvent& event) {
    std::cout << "KeyUp: " << static_cast<int>(event.keyCode) << std::endl;
}

ZUIHandle::ZUIHandle() : fZHandle(std::make_shared<ZUICommonHandle>()) {
}

void ZUIHandle::onUIEvent(const ZUIEvent& event) {
    if (!fZHandle) {
        return;
    }

    fZHandle->onUIEvent(event);
}

void ZUIHandle::setHandleEvent(std::shared_ptr<ZUIHandleEvent> handleEvent) {
    fZHandle = std::move(handleEvent);
}
