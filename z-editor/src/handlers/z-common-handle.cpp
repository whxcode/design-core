#include "z-editor/include/handlers/z-common-handle.h"

#include <iostream>

#include "z-editor/include/selection/z-selection.h"
#include "z-editor/include/z-editor-context.h"

bool ZUICommonHandle::onMouseDown(const ZUIEvent& event) {
    std::cout << "ZUICommonHandle::onMouseDown" << event.x << ", " << event.y << std::endl;
    return false;
}

bool ZUICommonHandle::onMouseMove(const ZUIEvent& event) {
    if (zContext && zContext->getSelection()) {
        zContext->getSelection()->hitHover(getCurrentPoint());
    }

    return false;
}

bool ZUICommonHandle::onMouseUp(const ZUIEvent& event) {
    if (!zContext || !zContext->getSelection()) {
        return false;
    }

    zContext->getSelection()->hitHover(getCurrentPoint());
    const auto hoverLayer = zContext->getSelection()->getHoverLayer();
    if (hoverLayer) {
        zContext->getSelection()->select(hoverLayer);
        return false;
    }

    zContext->getSelection()->clear();
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
