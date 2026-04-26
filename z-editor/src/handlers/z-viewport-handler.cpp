#include "z-editor/include/handlers/z-viewport-handler.h"

#include <algorithm>
#include <cmath>

#include "z-editor/include/z-editor-context.h"

ZViewportHandler::ZViewportHandler(ZEditorContext* context) : ZUIHandleEvent(context) {
}

bool ZViewportHandler::onMouseWheel(const ZUIEvent& event) {
    if (!zContext) {
        return false;
    }

    auto data = zContext->getViewportData();

    if (event.ctrlKey || event.metaKey) {
        const float oldScale = data.scale;
        const float zoomFactor = std::exp(-event.deltaY * 0.0015f);
        const float newScale = clampScale(oldScale * zoomFactor);

        if (newScale == oldScale) {
            return true;
        }

        const float worldX = (event.x - data.offsetX) / oldScale;
        const float worldY = (event.y - data.offsetY) / oldScale;

        data.scale = newScale;
        data.offsetX = event.x - worldX * newScale;
        data.offsetY = event.y - worldY * newScale;
    } else {
        data.offsetX -= event.deltaX;
        data.offsetY -= event.deltaY;
    }

    zContext->setViewportData(data);
    zContext->requestRedraw();
    return true;
}

float ZViewportHandler::clampScale(float scale) const {
    return std::clamp(scale, 0.05f, 64.0f);
}
