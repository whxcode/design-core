#pragma once
#include "z-wasm/include/z-wasm/z-js-value.h"

namespace {

template <typename T>
T readOptional(const emscripten::val& source, const char* key, T fallback) {
    const auto value = source[key];

    if (value.isUndefined() || value.isNull()) {
        return fallback;
    }

    return value.as<T>();
}

}  // namespace

namespace wasm::cpp {

ZUIEvent GetValue(const emscripten::val& source, ZUIEvent*) {
    ZUIEvent target;

    target.type = static_cast<ZUIEventType>(
        readOptional<int>(source, "type", static_cast<int>(ZUIEventType::zUnknown)));
    target.x = readOptional<float>(source, "x", 0.0f);
    target.y = readOptional<float>(source, "y", 0.0f);

    return target;
}

}  // namespace wasm::cpp
