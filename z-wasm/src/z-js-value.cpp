#include "z-wasm/include/z-wasm/z-js-value.h"

#include <cstddef>

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
    target.deltaX = readOptional<float>(source, "deltaX", 0.0f);
    target.deltaY = readOptional<float>(source, "deltaY", 0.0f);
    target.keyCode = static_cast<KeyCode>(readOptional<int>(source, "keyCode", 0));
    target.button = static_cast<MouseButton>(readOptional<int>(source, "button", 0));
    target.metaKey = readOptional<bool>(source, "metaKey", false);
    target.ctrlKey = readOptional<bool>(source, "ctrlKey", false);
    target.altKey = readOptional<bool>(source, "altKey", false);
    target.shiftKey = readOptional<bool>(source, "shiftKey", false);

    return target;
}

WindowContext GetValue(const emscripten::val& source, WindowContext*) {
    WindowContext context;

    context.zWidth = readOptional<size_t>(source, "width", size_t{1});
    context.zHeight = readOptional<size_t>(source, "height", size_t{1});
    context.zPixelWidth = readOptional<size_t>(source, "pixelWidth", size_t{1});
    context.zPixelHeight = readOptional<size_t>(source, "pixelHeight", size_t{1});
    context.zDpr = readOptional<float>(source, "dpr", 1.0f);

    return context;
}

}  // namespace wasm::cpp
