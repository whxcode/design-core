#include "z-paint/include/z-trace.h"

#include <algorithm>
#include <utility>

#include "z-app/include/ZAppEvent.h"
#include "z-editor/include/selection/z-selection.h"
#include "z-editor/include/z-editor-context.h"
#include "z-paint/include/shapes/z-hover-shape.h"
#include "z-paint/include/shapes/z-select-shape.h"
#include "z-paint/include/z-shape.h"

ZTrace::ZTrace(ZEditorContext* context, ZAppEvent* appEvent, RedrawCallback redrawCallback)
    : zContext(context), zAppEvent(appEvent), zRedrawCallback(std::move(redrawCallback)) {
    zHoverShape = MakeShape<ZHoverShape>(nullptr);
    addChild(zHoverShape);
    zSelectShape = MakeShape<ZSelectShape>();
    addChild(zSelectShape);

    if (!zAppEvent) {
        return;
    }

    zHoverLayerChangedListenerId =
        zAppEvent->on(ZAppEventType::zHoverLayerChanged, [this](const ZAppEventType) {
            syncHoverLayer();
            if (zRedrawCallback) {
                zRedrawCallback();
            }
        });
    zSelectedLayerChangedListenerId =
        zAppEvent->on(ZAppEventType::zSelectedLayerChanged, [this](const ZAppEventType) {
            syncSelectedLayers();
            if (zRedrawCallback) {
                zRedrawCallback();
            }
        });
}

ZTrace::~ZTrace() {
    if (!zAppEvent) {
        return;
    }

    if (zHoverLayerChangedListenerId != 0) {
        zAppEvent->off(ZAppEventType::zHoverLayerChanged, zHoverLayerChangedListenerId);
    }
    if (zSelectedLayerChangedListenerId != 0) {
        zAppEvent->off(ZAppEventType::zSelectedLayerChanged, zSelectedLayerChangedListenerId);
    }
}

ZGuid ZTrace::addChild(z_sp<ZShape> shape) {
    if (!shape) {
        return ZGuid::zInvalidId;
    }

    const auto id = shape->getId();
    zChildren.push_back(std::move(shape));
    return id;
}

void ZTrace::removeChild(const ZGuid& id) {
    std::erase_if(zChildren, [&id](const z_sp<ZShape>& shape) {
        return shape && shape->getId() == id;
    });
}

void ZTrace::clear() {
    zChildren.clear();
}

void ZTrace::syncHoverLayer() {
    if (!zContext || !zContext->getSelection()) {
        zHoverShape->setLayer(nullptr);
        return;
    }

    const auto hoverLayer = zContext->getSelection()->getHoverLayer();
    zHoverShape->setLayer(hoverLayer);
}

void ZTrace::syncSelectedLayers() {
    if (!zContext || !zContext->getSelection()) {
        zSelectShape->setRect(ZRect::MakeEmpty());
        return;
    }

    zSelectShape->setRect(zContext->getSelection()->getSelectedLayerWorldRect());
}

void ZTrace::render(IZEngine* engine, ZEditorContext* context) {
    for (const auto& shape : zChildren) {
        if (shape && shape->getVisible()) {
            shape->render(engine, context);
        }
    }
}
