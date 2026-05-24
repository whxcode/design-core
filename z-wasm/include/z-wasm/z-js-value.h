#pragma once

#include <emscripten/val.h>

#include "z-editor/include/ui-event/z-ui-event.h"
#include "z-window/include/ZWindow.h"

namespace wasm::cpp {

ZUIEvent GetValue(const emscripten::val& source, ZUIEvent* target);
WindowContext GetValue(const emscripten::val& source, WindowContext* target);

}  // namespace wasm::cpp
