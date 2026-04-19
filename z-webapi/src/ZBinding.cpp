#include <emscripten/bind.h>

#include "z-app/include/ZApp.h"

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
    class_<ZWindow>("Window").function("setTitle", &ZWindow::setTitle);

    // 绑定 Document
    class_<ZDocument>("Document").function("updatePosition", &ZDocument::setName);

    // 绑定 App：返回的是引用，符合你“掌握生命周期”的要求
    class_<ZApp>("App")
        .function("window", &ZApp::window, allow_raw_pointers())
        .function("document", &ZApp::document, allow_raw_pointers());

    // 暴力入口
    // 修改这里：调用辅助函数返回指针
    function("getApp", &getAppInstance, allow_raw_pointers());
}
