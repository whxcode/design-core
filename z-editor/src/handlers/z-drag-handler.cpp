#include "z-editor/include/handlers/z-drag-handler.h"

#include "z-document/include/commit/z-commit.h"
#include "z-document/include/layers/z-layerbase.h"
#include "z-document/include/models/z-layer-model.h"
#include "z-editor/include/selection/z-selection.h"
#include "z-editor/include/ui-event/z-ui-handle.h"
#include "z-editor/include/z-editor-context.h"

ZDragHandler::ZDragHandler(const ZHandlerType type, const ZUIHandleState& state,
                           ZEditorContext* context)
    : ZUIHandleEvent(type, state, context) {
    zLastPoint = getCurrentPoint();
}

bool ZDragHandler::onMouseMove(const ZUIEvent&) {
    const auto current = getCurrentPoint();
    const auto dx = current.x() - zLastPoint.x();
    const auto dy = current.y() - zLastPoint.y();
    if (dx == 0.0f && dy == 0.0f) {
        return true;
    }

    auto& selection = getSelection();
    for (const auto& layer : selection.getSelectedLayers()) {
        if (!layer) {
            continue;
        }

        const auto model = layer->getModel<ZLayerModel>();
        auto transform = layer->getWorldMatrix();
        transform.postTranslate(dx, dy);
        model->setTransform(layer->getParentInvertWorldMatrix() * transform);
    }

    zLastPoint = current;
    selection.refreshSelectedLayers();
    getContext().requestRedraw();
    return true;
}

bool ZDragHandler::onMouseUp(const ZUIEvent&) {
    getCommit().commit();
    getHandle().switchCommonHandler();
    getContext().requestRedraw();

    return true;
}
