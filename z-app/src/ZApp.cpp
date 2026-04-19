#include "z-app/include/ZApp.h"

#include <SDL2/SDL.h>

#include <iostream>
#include <thread>
#include <vector>

#include "z-document/include/ZDocument.h"
#include "z-window/include/ZWindow.h"

ZApp::ZApp() {
    // 掌握生命周期：在这里创建实例
    zWindow = std::make_unique<ZWindow>();
    zDocument = std::make_unique<ZDocument>();
}

ZWindow& ZApp::window() const {
    return *zWindow;
}

ZDocument& ZApp::document() const {
    return *zDocument;
}

void ZApp::startup() {
    std::cout << "App: Starting up..." << std::endl;
    std::vector<std::thread> threads{};
    for (size_t i = 0; i < 10; i++) {
        threads.emplace_back(std::thread{[i]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(5000));
            printf("i'm[%d],[%d]\n", i, std::this_thread::get_id());
        }});
    }

    for (auto& t : threads) {
        t.detach();
    }

    std::cout << "App: Startup sequence complete." << std::endl;

    this->zWindow->draw();
}
