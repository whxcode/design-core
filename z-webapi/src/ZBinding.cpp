#include <emscripten/bind.h>
#include <emscripten/val.h>

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

#include "z-app/include/ZApp.h"
#include "z-app/include/ZAppEvent.h"
#include "z-document/include/commit/z-commit.h"
#include "z-document/include/layers/z-component.h"
#include "z-document/include/layers/z-document.h"
#include "z-document/include/z-model-type.h"
#include "z-editor/include/command/z-command.h"
#include "z-kiwi/include/kiwi.h"
#include "z-kiwi/include/z-kiwi-reader.h"
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

namespace {

void collectModels(const z_sp<ZComponent>& comp, ZModelArray& models) {
    auto model = comp->getModel();
    if (model) {
        models.push_back(model);
    }

    for (const auto& child : comp->getChildren<ZComponent>()) {
        collectModels(child, models);
    }
}

val encodeModels(const ZModelArray& models) {
    kiwi::ByteBuffer bb;
    if (!ZKiwiWriter::encode(models, bb)) {
        return val::undefined();
    }

    auto view = typed_memory_view(bb.size(), bb.data());
    auto bytes = val::global("Uint8Array").new_(bb.size());
    bytes.call<void>("set", view);
    return bytes;
}

bool copyUint8Array(const val& src, std::vector<uint8_t>& out) {
    if (src.isNull() || src.isUndefined()) {
        return false;
    }

    auto byteArray = val::global("Uint8Array").new_(src);
    auto length = byteArray["length"].as<size_t>();
    if (length == 0) {
        return false;
    }

    out.resize(length);
    val bufferView = val(typed_memory_view(out.size(), out.data()));
    bufferView.call<void>("set", byteArray);
    return true;
}

bool appendDecodedModels(const val& src, ZModelArray& out) {
    std::vector<uint8_t> buffer;
    if (!copyUint8Array(src, buffer)) {
        return false;
    }

    kiwi::ByteBuffer bb(buffer.data(), buffer.size());
    auto models = ZKiwiReader::decode(bb);
    if (models.empty()) {
        return false;
    }

    out.insert(out.end(), models.begin(), models.end());
    return true;
}

bool hasPageModel(const ZModelArray& models) {
    for (const auto& model : models) {
        if (model && model->getType() == ZModelType::zPage) {
            return true;
        }
    }

    return false;
}

}  // namespace

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

                      ZModelArray documentModels;
                      if (auto model = doc.getModel()) {
                          documentModels.push_back(model);
                          result.set("name", model->getName());
                      }

                      auto documentBytes = encodeModels(documentModels);
                      if (!documentBytes.isUndefined()) {
                          result.set("document", documentBytes);
                      }

                      auto pages = val::array();
                      size_t pageIndex{0};
                      for (const auto& child : doc.getChildren<ZComponent>()) {
                          if (!child || child->getType() != ZModelType::zPage) {
                              continue;
                          }

                          ZModelArray pageModels;
                          collectModels(child, pageModels);
                          auto pageBytes = encodeModels(pageModels);
                          if (pageBytes.isUndefined()) {
                              continue;
                          }

                          auto page = val::object();
                          page.set("id", child->getModel()->getId().toString());
                          page.set("document", pageBytes);
                          pages.set(pageIndex++, page);
                      }
                      result.set("pages", pages);

                      return result;
                  }))

        .function("loadDocument", optional_override([](ZApp& self, val payload) -> val {
                      auto result = val::object();

                      if (payload.isNull() || payload.isUndefined()) {
                          result.set("success", false);
                          result.set("message", std::string("document bytes is empty"));
                          return result;
                      }

                      auto documentBytes = payload["document"];
                      if (documentBytes.isNull() || documentBytes.isUndefined()) {
                          ZModelArray models;
                          if (!appendDecodedModels(payload, models)) {
                              result.set("success", false);
                              result.set("message", std::string("document kiwi is invalid"));
                              return result;
                          }

                          if (!hasPageModel(models)) {
                              result.set("success", false);
                              result.set("message", std::string("page kiwi is empty"));
                              return result;
                          }

                          self.openDocument(models);

                          result.set("success", true);
                          result.set("models", models.size());
                          return result;
                      }

                      ZModelArray models;
                      if (!appendDecodedModels(documentBytes, models)) {
                          result.set("success", false);
                          result.set("message", std::string("document.kiwi is invalid"));
                          return result;
                      }

                      auto pages = payload["pages"];
                      size_t pageCount{0};
                      if (!pages.isNull() && !pages.isUndefined()) {
                          const auto length = pages["length"].as<size_t>();
                          for (size_t i = 0; i < length; i++) {
                              auto page = pages[i];
                              auto pageBytes = page["document"];
                              if (pageBytes.isNull() || pageBytes.isUndefined()) {
                                  pageBytes = page;
                              }
                              if (!appendDecodedModels(pageBytes, models)) {
                                  result.set("success", false);
                                  result.set("message", std::string("page kiwi is invalid"));
                                  return result;
                              }
                              pageCount++;
                          }
                      }

                      if (pageCount == 0) {
                          result.set("success", false);
                          result.set("message", std::string("page kiwi is empty"));
                          return result;
                      }

                      self.openDocument(models);

                      result.set("success", true);
                      result.set("models", models.size());
                      return result;
                  }));

    // 暴力入口
    // 修改这里：调用辅助函数返回指针
    function("getApp", &getAppInstance, allow_raw_pointers());
}
