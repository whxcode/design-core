#pragma once

#include <any>
#include <optional>

#include "z-document/include/commit/z-patch.h"
#include "z-document/include/prop/z-prop-key.h"
#include "z-tools/include/z-guid.h"

class ZComponent;

struct ZCollectItem {
    ZGuid zId{};
    ZPatchType zType{};
    std::set<ZPropKey> zUsed{};
    ZPatchProps zOldProps{};
    ZPatchProps zNewProps{};
};

class ZCollector {
public:
    void open();

    void close();

    bool canOpen();

public:
    void recordPropChanged(ZGuid id, ZPropKey key, std::any oldValue, std::any newValue);

    void recordPropChanged(const ZGuid id, const ZPropKey key, const void* const oldValue,
                           const void* const newValue);

    void recordAddChild(ZComponent* comp);

    void recordRemoveChild(ZComponent* comp);

    std::optional<ZPatch> commit();

    void clear();

    bool empty() const;

private:
    bool zEnable{true};
    ZCollectItem t;
    std::unordered_map<ZGuid, ZCollectItem, ZGuidHash> zCollectItems{};
};
