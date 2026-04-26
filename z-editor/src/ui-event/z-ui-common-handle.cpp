#include "z-editor/include/ui-event/z-ui-common-handle.h"

#include <iostream>

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
