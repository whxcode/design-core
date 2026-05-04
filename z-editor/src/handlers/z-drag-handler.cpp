#include "z-editor/include/handlers/z-drag-handler.h"

#include "z-document/include/commit/z-commit.h"
#include "z-document/include/layers/z-layerbase.h"
#include "z-document/include/models/z-layer-model.h"
#include "z-editor/include/selection/z-selection.h"
#include "z-editor/include/ui-event/z-ui-handle.h"
#include "z-editor/include/z-editor-context.h"

void ZDragHandler::setLastPoint(const ZPoint& point) {
    zLastPoint = point;
}

bool ZDragHandler::onMouseMove(const ZUIEvent&) {
    if (!zContext || !zContext->getSelection()) {
        return true;
    }

    const auto current = getCurrentPoint();
    const auto dx = current.x() - zLastPoint.x();
    const auto dy = current.y() - zLastPoint.y();
    if (dx == 0.0f && dy == 0.0f) {
        return true;
    }

    for (const auto& layer : zContext->getSelection()->getSelectedLayers()) {
        if (!layer) {
            continue;
        }

        const auto model = layer->getModel<ZLayerModel>();
        auto transform = layer->getWorldMatrix();
        transform.postTranslate(dx, dy);
        model->setTransform(layer->getParentInvertWorldMatrix() * transform);
    }

    zLastPoint = current;
    zContext->getSelection()->refreshSelectedLayers();
    zContext->requestRedraw();
    return true;
}

bool ZDragHandler::onMouseUp(const ZUIEvent&) {
    if (zContext && zContext->getCommit()) {
        zContext->getCommit()->commit();
    }

    if (zContext && zContext->getHandle()) {
        zContext->getHandle()->switchCommonHandler();
    }

    if (zContext) {
        zContext->requestRedraw();
    }

    return true;
}
