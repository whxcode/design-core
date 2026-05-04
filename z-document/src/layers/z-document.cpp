
#include "z-document/include/layers/z-document.h"

#include <cstdio>
#include <functional>
#include <iostream>

#include "z-document/include/layers/z-component.h"
#include "z-document/include/layers/z-layerbase.h"
#include "z-document/include/layers/z-page.h"
#include "z-document/include/z-model-type.h"

ZDocument::ZDocument(std::shared_ptr<ZDocumentModel> model) : ZComponent(model) {
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

std::vector<z_sp<ZLayerBase>> ZDocument::getNonPageLayers() const {
    std::vector<z_sp<ZLayerBase>> layers;

    const auto pageIt = zPages.find(zActivePageId);
    const auto page = zActivePageId != 0 && pageIt != zPages.end()
                          ? pageIt->second
                          : (zPages.empty() ? nullptr : zPages.begin()->second);
    if (!page) {
        return layers;
    }

    std::function<void(const z_sp<ZComponent>&)> visit = [&](const z_sp<ZComponent>& node) {
        if (!node) {
            return;
        }

        if (node->getType() != ZModelType::zPage) {
            layers.push_back(node->as<ZLayerBase>());
        }

        for (const auto& child : node->getChildren<ZComponent>()) {
            visit(child);
        }
    };

    visit(page);
    return layers;
}

void ZDocument::setActivePage(const size_t id) {
    zActivePageId = id;
};
