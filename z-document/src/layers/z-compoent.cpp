

#include <cstdio>
#include <iostream>

#include "z-document/include/layers/z-component.h"

ZComponent::ZComponent(z_sp<ZModel> model) : zModel(model) {
    zRuntimeId = model->getId().toNumber();
    zParentRuntimeId = model->getId().toNumber();
}

void ZComponent::addChild(const z_sp<ZComponent>& comp) {
    zLayers.push_back(comp);
}
