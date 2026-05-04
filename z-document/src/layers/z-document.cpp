
#include "z-document/include/layers/z-document.h"

#include <cstdio>
#include <iostream>

#include "z-document/include/layers/z-component.h"
#include "z-document/include/layers/z-layerbase.h"
#include "z-document/include/layers/z-page.h"
#include "z-document/include/z-model-type.h"

ZDocument::ZDocument(std::shared_ptr<ZDocumentModel> model)
    : ZComponent(model), zCollector(std::make_unique<ZCollector>()) {
}

void ZDocument::onModelPropChanged(const ZModel* const model, const ZPropKey key,
                                   const void* const oldValue, const void* const newValue) {
    zCollector->recordPropChanged(model->getId(), key, oldValue, newValue);
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
    for (const auto& item : patches) {
        const auto comp = findKey(item.zId);

        if (!comp) {
            continue;
        }

        comp->getModel()->setProps(item.zProps);
    }
}

void ZDocument::addChild(const z_sp<ZComponent>& comp) {
    const auto& model = comp->getModel();

    zLayers[comp->getUnique()] = comp;

    if (comp->getModel()->getType() == ZModelType::zPage) {
        zPages[comp->getUnique()] = comp->as<ZPage>();
    }

    ZComponent::addChild(comp);
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

void ZDocument::registerSubtree(const z_sp<ZComponent>& node) {
    if (!node) {
        return;
    }

    zLayers[node->getUnique()] = node;

    if (auto model = node->getModel()) {
        model->setChangeSink(this);
    }

    if (node->getType() == ZModelType::zPage) {
        zPages[node->getUnique()] = node->as<ZPage>();
    }

    for (const auto& child : node->getChildren<ZComponent>()) {
        registerSubtree(child);
    }
};
