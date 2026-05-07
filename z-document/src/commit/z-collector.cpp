#include "z-document/include/commit/z-collector.h"

#include "z-document/include/commit/z-patch.h"
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

void ZCollector::recordPropChanged(const ZGuid id, const ZPropKey key, const void* const oldValue,
                                   const void* const newValue) {
    auto oldAny = ZPropCodec::CopyValue(key, oldValue);
    auto newAny = ZPropCodec::CopyValue(key, newValue);

    if (!oldAny.has_value() || !newAny.has_value()) {
        return;
    }

    recordPropChanged(id, key, std::move(oldAny), std::move(newAny));
}

void ZCollector::recordAddChild(ZComponent* comp) {
    if (!zEnable || comp == nullptr) {
        return;
    }

    printf("ZCollector::recordAddChild\n");
}

void ZCollector::recordRemoveChild(ZComponent* comp) {
    if (!zEnable || comp == nullptr) {
        return;
    }
}

std::optional<ZPatch> ZCollector::commit() {
    if (zCollectItems.empty()) {
        return std::nullopt;
    }

    ZPatch patch;

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

    clear();

    return patch;
}

void ZCollector::clear() {
    zCollectItems.clear();
}

bool ZCollector::empty() const {
    return zCollectItems.empty();
}
