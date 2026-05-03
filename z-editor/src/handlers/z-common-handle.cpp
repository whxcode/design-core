#include "z-editor/include/handlers/z-common-handle.h"

#include <iostream>

bool ZUICommonHandle::onMouseDown(const ZUIEvent& event) {
    std::cout << "ZUICommonHandle::onMouseDown" << event.x << ", " << event.y << std::endl;
    return false;
}

bool ZUICommonHandle::onMouseMove(const ZUIEvent& event) {
    // std::cout << "ZUICommonHandle::MouseMove: " << event.x << ", " << event.y << std::endl;
    return false;
}

bool ZUICommonHandle::onMouseUp(const ZUIEvent& event) {
    // std::cout << "MouseUp: " << event.x << ", " << event.y << std::endl;
    return false;
}

bool ZUICommonHandle::onMouseWheel(const ZUIEvent& event) {
    // std::cout << "MouseWheel: " << event.deltaX << ", " << event.deltaY << std::endl;
    return false;
}

bool ZUICommonHandle::onKeyDown(const ZUIEvent& event) {
    // std::cout << "KeyDown: " << static_cast<int>(event.keyCode) << std::endl;
    return false;
}

bool ZUICommonHandle::onKeyUp(const ZUIEvent& event) {
    // std::cout << "KeyUp: " << static_cast<int>(event.keyCode) << std::endl;
    return false;
}
