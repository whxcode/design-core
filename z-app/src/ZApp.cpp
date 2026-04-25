#include "z-app/include/ZApp.h"

#include <SDL2/SDL.h>

#include <future>
#include <iostream>
#include <thread>
#include <vector>

#include "z-app/include/z-test-doc.h"
#include "z-document/include/creator/loader.h"
#include "z-matrix/include/z-matrix.h"
#include "z-tools/include/z-task.h"
#include "z-window/include/ZWindow.h"

ZApp::ZApp() {
    // 掌握生命周期：在这里创建实例
    zWindow = std::make_unique<ZWindow>();
    zWindow->setOverlayDrawer([this](IZEngine* engine) {
        renderImages(engine);
    });
}

ZWindow& ZApp::getWindow() const {
    return *zWindow;
}

ZDocument& ZApp::getDocument() const {
    return *zDocument;
}

void ZApp::startup() {
    auto futureR = DoTask([]() {
        return ZLoader::MakeDocument(ZTestDoc::MakeDoc());
    });

    printf("ZApp::startup\n");

    zDocument = futureR.get();
    std::cout << "document_ptr:" << zDocument << std::endl;

    auto page = zDocument->getActivePage();

    std::cout << "page_ptr:" << page << std::endl;

    this->zWindow->setComponent(page);
    this->zWindow->draw();
}

void ZApp::addImage(uintptr_t ptr, size_t size, float x, float y, float width, float height) {
    if (ptr == 0 || size == 0) {
        return;
    }

    zImages.push_back({.ptr = ptr, .size = size, .x = x, .y = y, .width = width, .height = height});
}

void ZApp::clearImages() {
    zImages.clear();
}

const std::vector<ZImagePayload>& ZApp::getImages() const {
    return zImages;
}

void ZApp::renderImages(IZEngine* engine) const {
    if (engine == nullptr) {
        return;
    }

    engine->save();
    ZMatrix m;

    for (const auto& image : zImages) {
        if (image.ptr == 0 || image.size == 0) {
            continue;
        }

        engine->transform(m.preTranslate(50, 50).preScale(200 / image.width, 100 / image.height));
        const auto* bytes = reinterpret_cast<const uint8_t*>(image.ptr);

        engine->drawImage(bytes, image.size, image.x, image.y, image.width, image.height);
    }

    engine->restore();
}

void ZApp::requestRedraw() {
    if (!zWindow) {
        return;
    }

    zWindow->draw();
}
