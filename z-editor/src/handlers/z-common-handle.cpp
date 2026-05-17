#include "z-editor/include/handlers/z-common-handle.h"

#include <iostream>

#include "z-editor/include/selection/z-selection.h"
#include "z-editor/include/ui-event/z-ui-handle.h"
#include "z-editor/include/z-editor-context.h"
#include "z-matrix/include/z-point.h"

bool ZUICommonHandle::onMouseDown(const ZUIEvent& event) {
    zDragPending = false;

    if (event.button != MouseButton::zLeft) {
        return false;
    }

    const auto selectedRect = zContext->getSelection()->getSelectedLayerWorldRect();
    if (!selectedRect.isEmpty() && selectedRect.contains(getCurrentPoint())) {
        zDragPending = true;
        return true;
    }

    zContext->getSelection()->hitHover(getCurrentPoint());
    if (zContext->getSelection()->getHoverLayer()) {
        return false;
    }

    return false;
}

bool ZUICommonHandle::onMouseMove(const ZUIEvent& event) {
    if (zDragPending) {
        zContext->getHandle()->switchDragHandler();
        zDragPending = false;
        return true;
    }

    zContext->getSelection()->hitHover(getCurrentPoint());

    if (isPressDown() &&  //
        ZPoint::Distance(getMouseDownPoint(), getCurrentPoint()) > 10.f) {
        zContext->getHandle()->switchSelectFrameHandler();
    }

    return false;
}

bool ZUICommonHandle::onMouseUp(const ZUIEvent& event) {
    zDragPending = false;

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
