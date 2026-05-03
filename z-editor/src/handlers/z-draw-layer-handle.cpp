#include "z-editor/include/handlers/z-draw-layer-handle.h"

#include <iostream>

bool ZDrawLayerHandle::onMouseDown(const ZUIEvent& event) {
    std::cout << "ZDrawPathHandle::onMouseDown" << std::endl;
    return false;
}

bool ZDrawLayerHandle::onMouseMove(const ZUIEvent& event) {
    // std::cout << "MouseMove: " << event.x << ", " << event.y << std::endl;
    return false;
}

bool ZDrawLayerHandle::onMouseUp(const ZUIEvent& event) {
    std::cout << "ZDrawPathHandle::onMouseUp" << std::endl;
    // std::cout << "MouseUp: " << event.x << ", " << event.y << std::endl;
    return false;
}

bool ZDrawLayerHandle::onMouseWheel(const ZUIEvent& event) {
    // std::cout << "MouseWheel: " << event.deltaX << ", " << event.deltaY << std::endl;
    return false;
}

bool ZDrawLayerHandle::onKeyDown(const ZUIEvent& event) {
    // std::cout << "KeyDown: " << static_cast<int>(event.keyCode) << std::endl;
    return false;
}

bool ZDrawLayerHandle::onKeyUp(const ZUIEvent& event) {
    // std::cout << "KeyUp: " << static_cast<int>(event.keyCode) << std::endl;
    return false;
}
