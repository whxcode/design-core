#pragma once

#include <any>
#include <set>
#include <unordered_map>
#include <vector>

#include "z-document/include/prop/z-prop-key.h"
#include "z-tools/include/z-guid.h"

using ZPatchValue = std::any;
using ZPatchProps = std::unordered_map<ZPropKey, ZPatchValue, ZPropKeyHash>;

enum class ZPatchType {
    zProps,   // 属性修改
    zAdd,     // 添加
    zRemove,  // 添加
};

struct ZPatchItem {
public:
    ZGuid zId{};
    ZPatchType zType{};
    ZPatchProps zProps{};
};

using ZPatches = std::vector<ZPatchItem>;

struct ZPatch {
    ZPatches zUndo{};
    ZPatches zRedo{};
};
