
#include "z-document/include/layers/z-document.h"

#include <cstdio>
#include <execution>
#include <iostream>

#include "z-document/include/commit/z-patch.h"
#include "z-document/include/creator/create-model.h"
#include "z-document/include/layers/z-component.h"
#include "z-document/include/layers/z-layerbase.h"
#include "z-document/include/layers/z-page.h"
#include "z-document/include/prop/z-prop-key.h"
#include "z-document/include/z-model-type.h"
#include "z-tools/include/z-assert.h"

ZDocument::ZDocument(std::shared_ptr<ZDocumentModel> model)
    : ZComponent(model), zCollector(std::make_unique<ZCollector>()) {
}

void ZDocument::onModelPropChanged(const ZModel* const model, const ZPropKey key,
                                   const void* const oldValue, const void* const newValue) {
    zCollector->recordPropChanged(model->getId(), key, oldValue, newValue);
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
    std::vector<ZPatchProps> newChildren;

    for (const auto& item : patches) {
        switch (item.zType) {
            case ZPatchType::zProps: {
                const auto comp = findKey(item.zId);
                if (!comp) {
                    continue;
                }

                comp->getModel()->setProps(item.zProps);

                break;
            }

            case ZPatchType::zRemove: {
                // 根据 id 找到节点，从父结点中移除？
                const auto comp = findKey(item.zId);
                if (!comp) {
                    continue;
                }

                if (!comp) {
                    Z_ASSERT(false, "未能找到将要移除的节点");
                    continue;
                }

                const auto parent = findKey(comp->getModel()->getParentId());

                if (!parent) {
                    Z_ASSERT(false, "未能找到将要移除的父节点");
                    continue;
                }

                parent->removeChild(comp);

                break;
            }

            case ZPatchType::zAdd: {
                newChildren.push_back(item.zProps);
                // printf("添加\n");
                break;
            }
        }
    }

    if (newChildren.empty()) {
        return;
    }

    for (const auto& props : newChildren) {
        auto typeIt = props.find(ZPropKey::zType);
        auto idIt = props.find(ZPropKey::zId);

        if (typeIt == props.end()) {
            Z_ASSERT(false, "未能找到将要添加的节点类型");
            return;
        }

        auto a = ZCreatorModel::Make(std::any_cast<const ZGuid&>(idIt->second),
                                     std::any_cast<const ZModelType&>(typeIt->second));
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

void ZDocument::unregisterSubtree(const z_sp<ZComponent>& node) {
    if (!node) {
        return;
    }

    zLayers.erase(node->getUnique());

    if (node->getType() == ZModelType::zPage) {
        zPages[node->getUnique()] = node->as<ZPage>();
    }

    if (auto model = node->getModel()) {
        model->setChangeSink(nullptr);
    }

    for (const auto& child : node->getChildren<ZComponent>()) {
        unregisterSubtree(child);
    }
};
