#include "z-editor/include/handlers/z-draw-layer-handle.h"

#include <iostream>

#include "z-editor/include/ui-event/z-ui-event.h"
#include "z-editor/include/ui-event/z-ui-handle.h"
#include "z-editor/include/z-editor-context.h"

bool ZDrawLayerHandle::onMouseDown(const ZUIEvent& event) {
    std::cout << "ZDrawPathHandle::onMouseDown" << std::endl;
    return true;
}

bool ZDrawLayerHandle::onMouseMove(const ZUIEvent& event) {
    // std::cout << "ZDrawLayerHandle::onMouseMove" << event.x << ", " << event.y << std::endl;
    return true;
}

bool ZDrawLayerHandle::onMouseUp(const ZUIEvent& event) {
    std::cout << "ZDrawPathHandle::onMouseUp" << std::endl;
    // std::cout << "MouseUp: " << event.x << ", " << event.y << std::endl;
    return true;
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
    if (event.keyCode == KeyCode::zEscape && zContext && zContext->getHandle()) {
        zContext->getHandle()->switchCommonHandler();
        return true;
    }

    return false;
}
