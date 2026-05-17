#include "z-editor/include/selection/z-selection.h"

#include <ranges>

#include "z-app/include/ZAppEvent.h"
#include "z-document/include/layers/z-document.h"
#include "z-document/include/layers/z-layerbase.h"
#include "z-document/include/layers/z-page.h"
#include "z-document/include/models/z-type.h"
#include "z-editor/include/z-editor-context.h"

ZSelection::ZSelection(ZEditorContext* context) : zContext(context) {
}

bool ZSelection::hitHover(const ZPoint& worldPoint) {
    const auto layer = hitTest(worldPoint);
    const auto nextId = layer ? layer->getModel()->getId() : ZGuid{};

    if (nextId == zHoverLayerId) {
        return false;
    }

    zHoverLayerId = nextId;
    emitHoverChanged();
    return true;
}

z_sp<ZLayerBase> ZSelection::getHoverLayer() const {
    if (!zContext || zHoverLayerId.toNumber() == 0) {
        return nullptr;
    }

    const auto* document = zContext->getDocument();
    if (!document) {
        return nullptr;
    }

    const auto component = document->findKey(zHoverLayerId);
    if (!component || component->getType() == ZModelType::zPage) {
        return nullptr;
    }

    return component->as<ZLayerBase>();
}

ZGuid ZSelection::getHoverLayerId() const {
    return zHoverLayerId;
}

const std::vector<z_sp<ZLayerBase>>& ZSelection::getSelectedLayers() const {
    return zSelectedLayers;
}

ZRect ZSelection::getSelectedLayerWorldRect() const {
    auto result = ZRect::MakeEmpty();

    for (const auto& layer : zSelectedLayers) {
        if (layer) {
            result.join(layer->getWorldRect());
        }
    }

    return result;
}

void ZSelection::clear() {
    if (zSelectedLayers.empty()) {
        return;
    }

    zSelectedLayers.clear();
    notice();
}

void ZSelection::select(const ZGuidArray& guids) {
    auto document = zContext->getDocument();
    const auto& layers = guids | std::ranges::views::transform([document](const ZGuid& guid) {
                             return document->findKey<ZLayerBase>(guid);
                         }) |
                         std::ranges::to<std::vector>();

    select(layers);
}

void ZSelection::select(const z_sp<ZLayerBase>& layer) {
    zSelectedLayers.clear();

    if (layer) {
        zSelectedLayers.push_back(layer);
    }

    emitSelectedChanged();
}

void ZSelection::select(const ZLayerBaseArray& layers) {
    const auto hadSelectedLayers = !zSelectedLayers.empty();

    zSelectedLayers.clear();
    append(layers);
    emitSelectedChanged();
}

void ZSelection::append(const z_sp<ZLayerBase>& layer) {
    if (!layer) {
        return;
    }

    const auto id = layer->getModel()->getId();
    if (containsSelectedLayer(id)) {
        return;
    }

    zSelectedLayers.push_back(layer);
    emitSelectedChanged();
}

void ZSelection::append(const ZLayerBaseArray& layers) {
    auto changed = false;

    for (const auto& layer : layers) {
        if (!layer) {
            continue;
        }

        const auto id = layer->getModel()->getId();
        if (containsSelectedLayer(id)) {
            continue;
        }

        zSelectedLayers.push_back(layer);
        changed = true;
    }

    if (changed) {
        emitSelectedChanged();
    }
}

void ZSelection::selectInRect(const ZRect& worldRect) {
    if (worldRect.isEmpty() || !zContext) {
        clear();
        return;
    }

    const auto page = zContext->getCurrentPage();
    if (!page) {
        clear();
        return;
    }

    std::vector<z_sp<ZLayerBase>> layers;
    for (const auto& child : page->getChildren<ZLayerBase>()) {
        collectLayersInRect(child, worldRect, layers);
    }

    select(layers);
}

void ZSelection::refreshSelectedLayers() {
    emitSelectedChanged();
}

ZGuidArray ZSelection::getSelectedLayerGuids() const {
    return zSelectedLayers | std::ranges::views::transform([](const auto& layer) {
               return layer->getModel()->getId();
           }) |
           std::ranges::to<std::vector>();
}

z_sp<ZLayerBase> ZSelection::hitTest(const ZPoint& worldPoint) const {
    if (!zContext) {
        return nullptr;
    }

    const auto page = zContext->getCurrentPage();
    if (!page) {
        return nullptr;
    }

    const auto layers = page->getChildren<ZLayerBase>();
    for (auto it = layers.rbegin(); it != layers.rend(); ++it) {
        const auto& layer = *it;
        if (layer && layer->hitTestWorldPoint(worldPoint)) {
            return layer;
        }
    }

    return nullptr;
}

void ZSelection::collectLayersInRect(const z_sp<ZLayerBase>& layer, const ZRect& worldRect,
                                     std::vector<z_sp<ZLayerBase>>& result) const {
    if (!layer) {
        return;
    }

    if (layer->getWorldRect().intersects(worldRect)) {
        result.push_back(layer);
    }

    for (const auto& child : layer->getChildren<ZLayerBase>()) {
        collectLayersInRect(child, worldRect, result);
    }
}

void ZSelection::emitHoverChanged() const {
    if (!zContext || !zContext->getAppEvent()) {
        return;
    }

    zContext->getAppEvent()->emit(ZAppEventType::zHoverLayerChanged);
}

void ZSelection::emitSelectedChanged() const {
    if (!zContext || !zContext->getAppEvent()) {
        return;
    }

    zContext->getAppEvent()->emit(ZAppEventType::zSelectedLayerChanged);
}

void ZSelection::notice() const {
    emitHoverChanged();
    emitSelectedChanged();
}

bool ZSelection::containsSelectedLayer(const ZGuid& id) const {
    for (const auto& layer : zSelectedLayers) {
        if (layer && layer->getModel()->getId() == id) {
            return true;
        }
    }

    return false;
}
