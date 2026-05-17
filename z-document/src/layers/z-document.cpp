
#include "z-document/include/layers/z-document.h"

#include <cstdio>

#include "z-document/include/commit/z-patch-merger.h"
#include "z-document/include/commit/z-patch.h"
#include "z-document/include/layers/z-component.h"
#include "z-document/include/layers/z-layerbase.h"
#include "z-document/include/layers/z-page.h"
#include "z-document/include/z-model-type.h"

ZDocument::ZDocument(std::shared_ptr<ZDocumentModel> model)
    : ZComponent(model), zCollector(std::make_unique<ZCollector>()) {
}

void ZDocument::onModelPropChanged(const ZGuid& id, const ZPropKey key, const PropValue& oldValue,
                                   const PropValue& newValue) {
    zCollector->recordPropChanged(id, key, oldValue, newValue);
}

void ZDocument::onAddChild(const z_sp<ZComponent>& comp) {
    if (!zCollector) {
        return;
    }

    zCollector->recordAddChild(comp);
    registerSubtree(comp);
}

void ZDocument::onRemoveChild(const z_sp<ZComponent>& comp) {
    if (!zCollector) {
        return;
    }

    zCollector->recordRemoveChild(comp);
    unregisterSubtree(comp);
}

void ZDocument::removeLayers(const ZLayerBaseArray& layers) {
    for (const auto& layer : layers) {
        auto parent = layer->getParent();

        if (parent == nullptr) {
            continue;
        }

        parent->removeChild(layer);
    }
}

void ZDocument::rebuildIndex() {
    zPages.clear();
    zLayers.clear();

    registerSubtree(as<ZComponent>());
}

void ZDocument::openCollector() {
    zCollector->open();
}

void ZDocument::closeCollector() {
    zCollector->close();
}

std::optional<ZPatch> ZDocument::commit() {
    return zCollector->commit();
}

void ZDocument::mergePatches(const ZPatches& patches) {
    ZPatchMerger::Merge(*this, patches);
}

void ZDocument::addChild(const z_sp<ZComponent>& comp) {
    ZComponent::addChild(comp);
}

void ZDocument::registerSubtree(const z_sp<ZComponent>& node) {
    if (!node) {
        return;
    }

    zLayers[node->getUnique()] = node;

    if (node->getType() == ZModelType::zPage) {
        zPages[node->getUnique()] = node->as<ZPage>();
    }

    if (auto model = node->getModel()) {
        model->setChangeSink(this);
    }

    for (const auto& child : node->getChildren<ZComponent>()) {
        registerSubtree(child);
    }
}

void ZDocument::unregisterSubtree(const z_sp<ZComponent>& node) {
    if (!node) {
        return;
    }

    zLayers.erase(node->getUnique());

    if (node->getType() == ZModelType::zPage) {
        zPages.erase(node->getUnique());
    }

    if (auto model = node->getModel()) {
        model->setChangeSink(nullptr);
    }

    for (const auto& child : node->getChildren<ZComponent>()) {
        unregisterSubtree(child);
    }
}

z_sp<ZPage> ZDocument::getActivePage() {
    if (zActivePageId == 0) {
        zActivePageId = zPages.begin()->second->getUnique();
    }

    return zPages[zActivePageId];
}

z_sp<ZComponent> ZDocument::findKey(const ZGuid id) const {
    return findKey(id.toNumber());
}

z_sp<ZComponent> ZDocument::findKey(const ZUniqueId id) const {
    const auto it = zLayers.find(id);

    if (it == zLayers.end()) {
        return nullptr;
    }

    return it->second;
}

void ZDocument::setActivePage(const ZUniqueId id) {
    zActivePageId = id;
}
