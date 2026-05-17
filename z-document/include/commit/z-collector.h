#pragma once

#include <any>
#include <functional>
#include <optional>
#include <set>
#include <unordered_map>
#include <unordered_set>

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
    void recordPropChanged(const ZGuid& id, const ZPropKey key, const PropValue& oldValue,
                           const PropValue& newValue);

    void recordAddChild(const z_sp<ZComponent>& comp);

    void recordRemoveChild(const z_sp<ZComponent>& comp);

    std::optional<ZPatch> commit();

    void clear();

    bool empty() const;

private:
    void collectorNewChild(ZPatch& patch);
    void collectorDiscardChild(ZPatch& patch);

private:
    bool zEnable{true};
    ZCollectItem t;

    // 图层属性变化节点
    std::unordered_map<ZGuid, ZCollectItem, ZGuidHash> zCollectItems{};

    // 新增图层节点。
    std::unordered_set<z_sp<ZComponent>> zNewChild{};

    // 删除图层节点。
    std::unordered_set<z_sp<ZComponent>> zDiscardChild{};
};
