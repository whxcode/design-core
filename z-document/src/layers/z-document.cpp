
#include "z-document/include/layers/z-document.h"

#include <cstdio>
#include <iostream>

#include "z-document/include/layers/z-component.h"
#include "z-document/include/layers/z-layerbase.h"
#include "z-document/include/layers/z-page.h"
#include "z-document/include/z-model-type.h"

ZDocument::ZDocument(std::shared_ptr<ZDocumentModel> model) : ZComponent(model) {
}

void ZDocument::addChild(const z_sp<ZComponent>& comp) {
    const auto& model = comp->getModel();

    printf("comp->getModel()->getType(): %d\n", static_cast<int>(comp->getModel()->getType()));

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

void ZDocument::setActivePage(const size_t id) {
    zActivePageId = id;
};
