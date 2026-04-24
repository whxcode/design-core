
#include <iostream>
#include <thread>

#include "z-app/include/ZApp.h"

extern "C" void force_link_webapi();

int main() {
    printf("--- [Wasm Core] Main Execution Started ---\n");
    force_link_webapi();  // 强制链接器加载 webapi 模块
    printf("force_link_webapi after\n");
    ZApp::Get().startup();
    return 0;
}
