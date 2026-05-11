#pragma once

#include <any>
#include <set>
#include <unordered_map>
#include <vector>

#include "z-document/include/prop/z-default-value.h"
#include "z-document/include/prop/z-prop-key.h"
#include "z-tools/include/z-guid.h"

using ZPatchValue = std::any;

class ZPatchProps {
public:
    template <ZPropKey P>
    void set(const typename PropTraits<P>::Type& value) {
        zValues[P] = value;
    }

    template <ZPropKey P>
    const typename PropTraits<P>::Type& get() const {
        return std::any_cast<const typename PropTraits<P>::Type&>(zValues.at(P));
    }

    template <ZPropKey P>
    bool contains() const {
        return zValues.contains(P);
    }

    auto find(ZPropKey key) const {
        return zValues.find(key);
    }

    auto end() const {
        return zValues.end();
    }

    auto begin() const {
        return zValues.begin();
    }

    auto end() {
        return zValues.end();
    }

    auto begin() {
        return zValues.begin();
    }

    size_t size() const {
        return zValues.size();
    }

private:
    std::unordered_map<ZPropKey, ZPatchValue, ZPropKeyHash> zValues{};
};

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
