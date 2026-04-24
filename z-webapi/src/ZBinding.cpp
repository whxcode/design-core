#pragma once
#include <emscripten/bind.h>
#include <emscripten/val.h>

#include <cstdint>
#include <cstdio>

#include "z-app/include/ZApp.h"
#include "z-document/include/layers/z-document.h"

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
    // 绑定 Window
    class_<ZWindow>("Window")
        .function("setTitle", &ZWindow::setTitle)
        .function("dump", &ZWindow::dump);

    // 绑定 Document
    class_<ZDocument>("Document").function("setName", &ZDocument::setName);

    // 绑定 App：返回的是引用，符合你“掌握生命周期”的要求
    class_<ZApp>("App")
        .function("calloc", optional_override([](ZApp& self, size_t size) -> uintptr_t {
                      void* ptr = std::malloc(size);
                      printf("calloc size: %zu, ptr: %p\n", size, ptr);
                      return reinterpret_cast<uintptr_t>(ptr);
                  }))

        .function("putImage1", optional_override([](ZApp& self, val buffer) -> void {
                      auto byte = vecFromJSArray<uint8_t>(buffer);
                      printf("putImage1[%d],[%d]\n", &byte, byte.size());
                  }))

        .function("putImage2", optional_override([](ZApp& self, uintptr_t ptr, size_t len) -> void {
                      printf("putImage2[%d],[%d]\n", ptr, len);
                  }))

        .function("window", optional_override([](ZApp& self) {
                      return &self.getWindow();
                  }),
                  allow_raw_pointers())
        .function("document", optional_override([](ZApp& self) {
                      return &self.getDocument();
                  }),
                  allow_raw_pointers());

    // 暴力入口
    // 修改这里：调用辅助函数返回指针
    function("getApp", &getAppInstance, allow_raw_pointers());
}
