

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

    const auto sink = this->getModel()->getChangeSink();

    // printf("sink[%d]\n", sink);
    if (sink != nullptr) {
        sink->onAddChild(comp);
    }
}

void ZComponent::removeChild(const z_sp<ZComponent>& comp) {
    // 移除子节点.
    comp->zParent.reset();

    std::erase_if(zLayers, [comp](const z_sp<ZComponent>& child) {
        return child->getModel()->getId() == comp->getModel()->getId();
    });

    const auto sink = this->getModel()->getChangeSink();

    // printf("sink[%d]\n", sink);
    if (sink != nullptr) {
        sink->onRemoveChild(comp);
    }
};

z_sp<ZComponent> ZComponent::getParent() const {
    return zParent.lock();
}
