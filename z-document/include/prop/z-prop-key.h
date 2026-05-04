#pragma once

#include <cstddef>
enum class ZPropKey {
    zId,
    zType,
    zParentId,
    zName,
    zSize,
    zTransform,
    zFillColor,
};

struct ZPropKeyHash {
    ZPropKeyHash() = default;
    size_t operator()(ZPropKey propKey) const noexcept {
        return static_cast<size_t>(propKey);
    }
};
