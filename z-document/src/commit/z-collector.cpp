#include "z-document/include/commit/z-collector.h"

#include <iostream>
#include <optional>

#include "z-document/include/commit/z-patch.h"
#include "z-document/include/layers/z-component.h"
#include "z-document/include/models/z-type.h"
#include "z-document/include/prop/z-prop-codec.h"

namespace {

void SetPatchProp(ZPatchProps& props, ZPropKey key, const std::any& value) {
    switch (key) {
        case ZPropKey::zId:
            props.set<ZPropKey::zId>(std::any_cast<const ZGuid&>(value));
            return;
        case ZPropKey::zType:
            props.set<ZPropKey::zType>(std::any_cast<const ZModelType&>(value));
            return;
        case ZPropKey::zParentId:
            props.set<ZPropKey::zParentId>(std::any_cast<const ZGuid&>(value));
            return;
        case ZPropKey::zName:
            props.set<ZPropKey::zName>(std::any_cast<const std::string&>(value));
            return;
        case ZPropKey::zSize:
            props.set<ZPropKey::zSize>(std::any_cast<const ZSize&>(value));
            return;
        case ZPropKey::zTransform:
            props.set<ZPropKey::zTransform>(std::any_cast<const ZMatrix&>(value));
            return;
        case ZPropKey::zFillColor:
            props.set<ZPropKey::zFillColor>(std::any_cast<const uint32_t&>(value));
            return;
    }
}

}  // namespace

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
        SetPatchProp(item.zOldProps, key, oldValue);
        item.zUsed.insert(key);
    }

    SetPatchProp(item.zNewProps, key, newValue);
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
