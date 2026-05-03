#pragma once

#include <emscripten/val.h>

#include "z-editor/include/ui-event/z-ui-event.h"

namespace wasm::cpp {

ZUIEvent GetValue(const emscripten::val& source, ZUIEvent* target);

}  // namespace wasm::cpp
