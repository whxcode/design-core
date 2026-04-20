#include "z-app/include/ZApp.h"

#include <SDL2/SDL.h>

#include <future>
#include <iostream>
#include <thread>
#include <vector>

#include "z-app/include/z-test-doc.h"
#include "z-document/include/creator/loader.h"
#include "z-tools/include/z-task.h"
#include "z-window/include/ZWindow.h"

ZApp::ZApp() {
    // 掌握生命周期：在这里创建实例
    zWindow = std::make_unique<ZWindow>();
}

ZWindow& ZApp::window() const {
    return *zWindow;
}

ZDocument& ZApp::document() const {
    return *zDocument;
}

void ZApp::startup() {
    auto futureR = DoTask([]() {
        return ZLoader::MakeDocument(ZTestDoc::MakeDoc());
    });

    zDocument = futureR.get();
    std::cout << "document_ptr:" << zDocument << std::endl;

    this->zWindow->draw();
}
