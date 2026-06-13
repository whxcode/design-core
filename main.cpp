
#include <iostream>
#include <thread>

#include "z-app/include/ZApp.h"

extern "C" void force_link_wasmapi();

int main() {
    printf("__EMSCRIPTEN__[%d]\n", __EMSCRIPTEN__);
    printf("--- [Wasm Core] Main Execution Started ---\n");
    force_link_wasmapi();  // 强制链接器加载 wasmapi 模块
    printf("force_link_wasmapi after\n");
    ZApp::Get().startup();
    return 0;
}
