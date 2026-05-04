

#include <cstdio>
#include <iostream>

#include "z-document/include/layers/z-component.h"

ZComponent::ZComponent(z_sp<ZModel> model) : zModel(model) {
    zRuntimeId = model->getId().toNumber();
    zParentRuntimeId = model->getParentId().toNumber();
}

void ZComponent::addChild(const z_sp<ZComponent>& comp) {
    if (comp) {
        comp->zParent = shared_from_this();
    }

    zLayers.push_back(comp);
}

z_sp<ZComponent> ZComponent::getParent() const {
    return zParent.lock();
}
