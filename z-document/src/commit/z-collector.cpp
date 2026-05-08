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

void ZCollector::recordPropChanged(const ZGuid id, const ZPropKey key, const void* const oldValue,
                                   const void* const newValue) {
    auto oldAny = ZPropCodec::CopyValue(key, oldValue);
    auto newAny = ZPropCodec::CopyValue(key, newValue);

    if (!oldAny.has_value() || !newAny.has_value()) {
        return;
    }

    recordPropChanged(id, key, std::move(oldAny), std::move(newAny));
}

void ZCollector::recordPropChanged(ZGuid id, ZPropKey key, std::any oldValue, std::any newValue) {
    if (!zEnable) {
        return;
    }

    // 收集属性.
    auto& item = zCollectItems[id];
    item.zId = id;
    item.zType = ZPatchType::zProps;

    if (!item.zUsed.contains(key)) {
        item.zOldProps[key] = std::move(oldValue);
        item.zUsed.insert(key);
    }

    item.zNewProps[key] = std::move(newValue);
}

void ZCollector::recordAddChild(const z_sp<ZComponent>& comp) {
    if (!zEnable || comp == nullptr) {
        return;
    }

    // printf("ZCollector::recordAddChild[添加]\n");
    zNewChild.insert(comp.get());
}

void ZCollector::recordRemoveChild(const z_sp<ZComponent>& comp) {
    if (!zEnable || comp == nullptr) {
        return;
    }
}

std::optional<ZPatch> ZCollector::commit() {
    if (zCollectItems.empty() &&  //
        zNewChild.empty()) {
        printf("empty patch\n");
        return std::nullopt;
    }

    ZPatch patch;
    ZComponentArray comps;

    printf("zNewChild[%d]\n", zNewChild.size());
    for (const auto& comp : zNewChild) {
        comps.push_back(comp->as<ZComponent>());

        const auto& model = comp->getModel();

        patch.zUndo.push_back({
            .zId = model->getId(),
            .zType = ZPatchType::zRemove,
            .zProps = {},
        });
    }

    ZPropCodec::DeepBST(comps, [&patch](const z_sp<ZComponent>& comp) {
        const auto& model = comp->getModel();

        patch.zRedo.push_back({
            .zId = model->getId(),
            .zType = ZPatchType::zAdd,
            .zProps = ZPropCodec::MakeProps(model),
        });
    });

    for (const auto& [_, item] : zCollectItems) {
        patch.zUndo.push_back({
            .zId = item.zId,
            .zType = item.zType,
            .zProps = std::move(item.zOldProps),
        });

        patch.zRedo.push_back({
            .zId = item.zId,
            .zType = item.zType,
            .zProps = std::move(item.zNewProps),
        });
    }

    // std::cout << "patch::undo:" << patch.zUndo.size() << std::endl;

    clear();

    return patch;
}

void ZCollector::clear() {
    zCollectItems.clear();
    zNewChild.clear();
}

bool ZCollector::empty() const {
    return zCollectItems.empty();
}
