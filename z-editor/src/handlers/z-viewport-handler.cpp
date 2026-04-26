#include "z-editor/include/handlers/z-viewport-handler.h"

#include <algorithm>
#include <cmath>

#include "z-editor/include/z-editor-context.h"

ZViewportHandler::ZViewportHandler(ZEditorContext* context) : ZUIHandleEvent(context) {
}

bool ZViewportHandler::onMouseDown(const ZUIEvent& event) {
    if (!zSpacePressed || event.button != MouseButton::zLeft) {
        return false;
    }

    zSpacePanning = true;
    zLastX = event.x;
    zLastY = event.y;
    return true;
}

bool ZViewportHandler::onMouseMove(const ZUIEvent& event) {
    if (!zSpacePanning) {
        return false;
    }

    panBy(event.x - zLastX, event.y - zLastY);
    zLastX = event.x;
    zLastY = event.y;
    return true;
}

bool ZViewportHandler::onMouseUp(const ZUIEvent& event) {
    if (!zSpacePanning || event.button != MouseButton::zLeft) {
        return false;
    }

    zSpacePanning = false;
    return true;
}

bool ZViewportHandler::onMouseWheel(const ZUIEvent& event) {
    if (!zContext) {
        return false;
    }

    if (event.ctrlKey || event.metaKey) {
        zoomAt(event.x, event.y, event.deltaY);
    } else if (event.shiftKey) {
        const float delta = event.deltaX != 0.0f ? event.deltaX : event.deltaY;
        panBy(-scrollDistance(delta), 0.0f);
    } else {
        panBy(-scrollDistance(event.deltaX), -scrollDistance(event.deltaY));
    }

    return true;
}

bool ZViewportHandler::onKeyDown(const ZUIEvent& event) {
    if (event.keyCode != KeyCode::zSpace) {
        return false;
    }

    zSpacePressed = true;
    return false;
}

bool ZViewportHandler::onKeyUp(const ZUIEvent& event) {
    if (event.keyCode != KeyCode::zSpace) {
        return false;
    }

    zSpacePressed = false;
    zSpacePanning = false;
    return false;
}

float ZViewportHandler::clampScale(float scale) const {
    return std::clamp(scale, 0.05f, 64.0f);
}

float ZViewportHandler::scrollDistance(float delta) const {
    if (delta == 0.0f) {
        return 0.0f;
    }

    const float sign = delta > 0.0f ? 1.0f : -1.0f;
    return sign * std::log1p(std::abs(delta)) * 14.0f;
}

float ZViewportHandler::zoomFactor(float delta) const {
    const float normalized = scrollDistance(delta);
    return std::exp(-normalized * 0.015f);
}

void ZViewportHandler::panBy(float dx, float dy) const {
    if (!zContext) {
        return;
    }

    auto data = zContext->getViewportData();
    data.offsetX += dx;
    data.offsetY += dy;
    zContext->setViewportData(data);
}

void ZViewportHandler::zoomAt(float x, float y, float delta) const {
    if (!zContext) {
        return;
    }

    auto data = zContext->getViewportData();
    const float oldScale = data.scale;
    const float newScale = clampScale(oldScale * zoomFactor(delta));

    if (newScale == oldScale) {
        return;
    }

    const float worldX = (x - data.offsetX) / oldScale;
    const float worldY = (y - data.offsetY) / oldScale;

    data.scale = newScale;
    data.offsetX = x - worldX * newScale;
    data.offsetY = y - worldY * newScale;
    zContext->setViewportData(data);
}
