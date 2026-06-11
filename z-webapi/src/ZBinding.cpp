#include <emscripten/bind.h>
#include <emscripten/val.h>

#include <cstdint>
#include <cstdio>
#include <cstring>

#include "z-app/include/ZApp.h"
#include "z-app/include/ZAppEvent.h"
#include "z-document/include/commit/z-commit.h"
#include "z-document/include/layers/z-document.h"
#include "z-editor/include/command/z-command.h"
#include "z-kiwi/include/z-kiwi-writer.h"
#include "z-tools/include/z-editor-theme.h"
#include "z-wasm/include/z-wasm/z-js-value.h"

// 在文件末尾或开头添加一个空函数
extern "C" {
void force_link_webapi() {
}
}

using namespace emscripten;

// 在 webapi/src/Binding.cpp 顶部定义一个辅助函数
static ZApp* getAppInstance() {
    return &ZApp::Get();
}

EMSCRIPTEN_BINDINGS(core_api) {
    class_<ZAppEvent>("AppEvent")
        .function("on", optional_override(
                            [](ZAppEvent& self, const int type, const val& callback) -> size_t {
                                return self.on(static_cast<ZAppEventType>(type),
                                               [callback](ZAppEventType eventType) {
                                                   callback(static_cast<uint32_t>(eventType));
                                               });
                            }))
        .function("off",
                  optional_override([](ZAppEvent& self, const int type, const size_t id) -> void {
                      self.off(static_cast<ZAppEventType>(type), id);
                  }));

    value_object<ViewportData>("ViewportData")
        .field("offsetX", &ViewportData::offsetX)
        .field("offsetY", &ViewportData::offsetY)
        .field("scale", &ViewportData::scale);

    // 绑定 Window
    class_<ZWindow>("Window")
        .function("dump", &ZWindow::dump)
        .function(
            "setContext", optional_override([](ZWindow& self, const val& value) {
                self.setContext(wasm::cpp::GetValue(value, static_cast<WindowContext*>(nullptr)));
            }));

    // 绑定 Document
    class_<ZDocument>("Document").function("setName", &ZDocument::setName);

    class_<ZCommand>("Command")
        .function("canExecute", optional_override([](ZCommand& self, const int type) -> bool {
                      return self.canExecute(static_cast<ZCommandType>(type));
                  }))
        .function("execute", optional_override([](ZCommand& self, const int type) -> void {
                      self.execute(static_cast<ZCommandType>(type));
                  }));

    // 绑定 App：返回的是引用，符合你“掌握生命周期”的要求
    class_<ZApp>("App")
        .function("putImage", optional_override([](ZApp& self, const size_t size, const size_t w,
                                                   const size_t h) -> uintptr_t {
                      auto ptr = reinterpret_cast<uintptr_t>(std::malloc(size));
                      std::memset(reinterpret_cast<void*>(ptr), 0, size);

                      self.addImage(ptr, size, 0, 0, w, h);
                      return ptr;
                  }))

        .function("draw", optional_override([](ZApp& self) -> void {
                      self.requestRedraw();
                  }))

        .function("viewport", optional_override([](ZApp& self) -> ViewportData {
                      return self.getViewportData();
                  }))

        .function("handler", optional_override([](ZApp& self) -> int {
                      return static_cast<int>(self.getHandlerType());
                  }))

        .function("editorMode", optional_override([](ZApp& self) -> int {
                      return static_cast<int>(self.getEditorModeType());
                  }))

        .function("onUIEvent", optional_override([](ZApp& self, const val& event) -> void {
                      self.onUIEvent(wasm::cpp::GetValue(event, static_cast<ZUIEvent*>(nullptr)));
                  }))

        .function("setTheme", optional_override([](ZApp& self, const int type) -> void {
                      self.setTheme(static_cast<ZEditorThemeType>(type));
                  }))

        .function("appEvent", optional_override([](ZApp& self) -> ZAppEvent* {
                      return &self.getAppEvent();
                  }),
                  allow_raw_pointers())

        .function("command", optional_override([](ZApp& self) -> ZCommand* {
                      return &self.getCommand();
                  }),
                  allow_raw_pointers())

        .function("window", optional_override([](ZApp& self) {
                      return &self.getWindow();
                  }),
                  allow_raw_pointers())
        .function("document", optional_override([](ZApp& self) {
                      return &self.getDocument();
                  }),
                  allow_raw_pointers())

        .function("exportDocument", optional_override([](ZApp& self) -> val {
                      auto& doc = self.getDocument();
                      auto result = val::object();

                      ZModelArray models;
                      doc.collectExportModels(models);

                      kiwi::ByteBuffer bb;
                      if (ZKiwiWriter::encode(models, bb)) {
                          auto view = typed_memory_view(bb.size(), bb.data());
                          auto docArr = val::global("Uint8Array").new_(bb.size());
                          docArr.call<void>("set", view);
                          result.set("document", docArr);
                      }

                      return result;
                  }))

        .function("loadDocument", optional_override([](ZApp& self, val bytes) -> val {
                      auto length = bytes["length"].as<size_t>();
                      std::vector<uint8_t> buffer(length);

                      /*
                                      val memoryView = val::global("Uint8Array")
                                                           .new_(val::module_property("HEAPU8").call<val>(
                                                               "subarray", bytes["byteOffset"],
                                                               bytes["byteOffset"].as<size_t>() +
                         length)); memoryView.call<void>("set",
                                                            val::global("Uint8Array").new_(buffer.data(),
                         length));
                      */

                      printf("size(%d)\n", buffer.size());

                      auto result = val::object();
                      result.set("success", true);  // 假设加载成功
                      return result;
                  }));

    // 暴力入口
    // 修改这里：调用辅助函数返回指针
    function("getApp", &getAppInstance, allow_raw_pointers());
}
