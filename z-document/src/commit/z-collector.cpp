#include "z-document/include/commit/z-collector.h"

#include <iostream>
#include <optional>

#include "z-document/include/commit/z-patch.h"
#include "z-document/include/layers/z-component.h"
#include "z-document/include/models/z-type.h"
#include "z-document/include/prop/z-prop-codec.h"

void ZCollector::open() {
    zEnable = true;
}

void ZCollector::close() {
    zEnable = false;
}

bool ZCollector::canOpen() {
    return zEnable;
}

void ZCollector::recordPropChanged(const ZGuid& id, const ZPropKey key, const PropValue& oldValue,
                                   const PropValue& newValue) {
    if (!zEnable) {
        return;
    }

    // 收集属性.
    auto& item = zCollectItems[id];
    item.zId = id;
    item.zType = ZPatchType::zProps;

    if (!item.zUsed.contains(key)) {
        item.zOldProps.setAny(key, oldValue);

        item.zUsed.insert(key);
    }

    item.zNewProps.setAny(key, newValue);
}

void ZCollector::recordAddChild(const z_sp<ZComponent>& comp) {
    if (!zEnable || comp == nullptr) {
        return;
    }

    zDiscardChild.erase(comp);
    zNewChild.insert(comp);
}

void ZCollector::recordRemoveChild(const z_sp<ZComponent>& comp) {
    if (!zEnable || comp == nullptr) {
        return;
    }

    zNewChild.erase(comp);
    zDiscardChild.insert(comp);
}

void ZCollector::collectorNewChild(ZPatch& patch) {
    ZComponentArray comps;

    for (const auto& comp : zNewChild) {
        comps.push_back(comp);

        const auto& model = comp->getModel();

        patch.zUndo.zPatches.push_back({
            .zId = model->getId(),
            .zType = ZPatchType::zRemove,
            .zProps = {},
        });
    }

    ZPropCodec::DeepBST(comps, [&patch](const z_sp<ZComponent>& comp) {
        const auto& model = comp->getModel();

        patch.zRedo.zPatches.push_back({
            .zId = model->getId(),
            .zType = ZPatchType::zAdd,
            .zProps = ZPropCodec::MakeProps(model),
        });
    });
}

void ZCollector::collectorDiscardChild(ZPatch& patch) {
    ZComponentArray comps;

    for (const auto& comp : zDiscardChild) {
        comps.push_back(comp);

        const auto& model = comp->getModel();

        patch.zRedo.zPatches.push_back({
            .zId = model->getId(),
            .zType = ZPatchType::zRemove,
            .zProps = {},
        });
    }

    ZPropCodec::DeepBST(comps, [&patch](const z_sp<ZComponent>& comp) {
        const auto& model = comp->getModel();

        patch.zUndo.zPatches.push_back({
            .zId = model->getId(),
            .zType = ZPatchType::zAdd,
            .zProps = ZPropCodec::MakeProps(model),
        });
    });
}

std::optional<ZPatch> ZCollector::commit() {
    if (zCollectItems.empty() &&  //
        zNewChild.empty() &&      //
        zDiscardChild.empty()) {
        printf("empty patch\n");
        return std::nullopt;
    }

    ZPatch patch;

    collectorNewChild(patch);
    collectorDiscardChild(patch);

    for (const auto& [_, item] : zCollectItems) {
        patch.zUndo.zPatches.push_back({
            .zId = item.zId,
            .zType = item.zType,
            .zProps = std::move(item.zOldProps),
        });

        patch.zRedo.zPatches.push_back({
            .zId = item.zId,
            .zType = item.zType,
            .zProps = std::move(item.zNewProps),
        });
    }

    clear();

    return patch;
}

void ZCollector::clear() {
    zCollectItems.clear();
    zNewChild.clear();
    zDiscardChild.clear();
}

bool ZCollector::empty() const {
    return zCollectItems.empty();
}
