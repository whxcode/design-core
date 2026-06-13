#pragma once

#include <any>
#include <functional>
#include <set>
#include <unordered_map>
#include <vector>

#include "z-document/include/prop/z-default-value.h"
#include "z-document/include/prop/z-prop-key.h"
#include "z-document/include/prop/z-sparse-props.h"
#include "z-tools/include/z-guid.h"

class ZPatchProps {
public:
    ZPatchProps() = default;
    ZPatchProps(const SparseProps& props) : zProps(props) {};

public:
    void setAny(const ZPropKey key, const PropValue& value) {
        zProps.setAny(key, value);
    }

    template <ZPropKey P>
    const typename PropTraits<P>::Type& get() const {
        return zProps.get<P>();
    }

    const SparseProps& getProps() const {
        return zProps;
    }

    const SparseProps::Storages& getEntry() const {
        return zProps.getEntry();
    }

private:
    SparseProps zProps{};
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

struct ZPatches {
    std::vector<ZPatchItem> zPatches;
    std::vector<ZGuid> zSelectedLayers{};
};

using ZPatchHandler = std::function<void()>;

struct ZPatch {
    ZPatches zUndo{};
    ZPatches zRedo{};
};
