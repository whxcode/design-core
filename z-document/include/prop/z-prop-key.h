#pragma once

#include <cstddef>
enum class ZPropKey {
    zId,
    zType,
    zParentId,
    zName,
    zSize,
    zTransform,
    zFills,
    zStrokes,
    zStartAngle,
    zEndAngle,
    zInnerRadius,
    zPaths,
    zWindingRule,
};

struct ZPropKeyHash {
    ZPropKeyHash() = default;
    size_t operator()(ZPropKey propKey) const noexcept {
        return static_cast<size_t>(propKey);
    }
};
