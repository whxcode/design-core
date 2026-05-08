#pragma once

#include <any>
#include <optional>

#include "z-document/include/commit/z-patch.h"
#include "z-document/include/prop/z-prop-key.h"
#include "z-tools/include/z-guid.h"
#include "z-tools/include/z-type.h"

class ZComponent;

struct ZCollectNode {
    ZPatchType zType{};
    ZPatchProps props{};  // 存入添加节点/删除节点的props数据。
};

struct ZCollectItem {
    ZGuid zId{};
    ZPatchType zType{};
    std::set<ZPropKey> zUsed{};
    ZPatchProps zOldProps{};
    ZPatchProps zNewProps{};

    // 收集子节点信息
    std::unordered_map<ZGuid, ZCollectNode, ZGuidHash> zCollectChildren{};
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

    void recordAddChild(const z_sp<ZComponent>& comp);

    void recordRemoveChild(const z_sp<ZComponent>& comp);

    std::optional<ZPatch> commit();

    void clear();

    bool empty() const;

private:
    bool zEnable{true};
    ZCollectItem t;
    std::unordered_map<ZGuid, ZCollectItem, ZGuidHash> zCollectItems{};
    std::set<ZComponent*> zNewChild{};
};
