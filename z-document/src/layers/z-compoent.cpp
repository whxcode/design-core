

#include <cstdio>
#include <iostream>

#include "z-document/include/layers/z-component.h"
#include "z-document/include/models/z-document-change-sink.h"

ZComponent::ZComponent(z_sp<ZModel> model) : zModel(model) {
    zRuntimeId = model->getId().toNumber();
    zParentRuntimeId = model->getParentId().toNumber();
}

void ZComponent::addChild(const z_sp<ZComponent>& comp) {
    if (!comp) {
        return;
    }

    comp->getModel()->setParentId(zModel->getId());
    comp->zParent = shared_from_this();
    zLayers.push_back(comp);

    const auto sink = comp->getModel()->getChangeSink();

    if (sink != nullptr) {
        sink->onAddChild(comp.get());
    }
}

z_sp<ZComponent> ZComponent::getParent() const {
    return zParent.lock();
}
