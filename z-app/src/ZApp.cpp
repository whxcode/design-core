#include "z-app/include/ZApp.h"

#include <SDL2/SDL.h>

#include <future>
#include <iostream>
#include <thread>
#include <vector>

#include "z-app/include/z-test-doc.h"
#include "z-document/include/creator/loader.h"
#include "z-matrix/include/z-matrix.h"
#include "z-paint/include/z-shape.h"
#include "z-tools/include/z-task.h"
#include "z-window/include/ZWindow.h"

ZApp::ZApp() {
    // 掌握生命周期：在这里创建实例
    zWindow = std::make_unique<ZWindow>();
}

ZWindow& ZApp::getWindow() const {
    return *zWindow;
}

ZDocument& ZApp::getDocument() const {
    return *zDocument;
}

ViewportData ZApp::getViewportData() const {
    if (!zEditorContext) {
        return {};
    }

    return zEditorContext->getViewportData();
}

ZHandlerType ZApp::getHandlerType() const {
    if (!zUIHandle) {
        return ZHandlerType::zCommon;
    }

    return zUIHandle->activeHandlerType();
}

void ZApp::switchHandler(const ZHandlerType type) {
    if (!zUIHandle) {
        return;
    }

    zUIHandle->switchHandler(type);
    requestRedraw();
}

void ZApp::startup() {
    auto futureR = DoTask([]() {
        return ZLoader::MakeDocument(ZTestDoc::MakeDoc());
    });

    zDocument = futureR.get();

    auto page = zDocument->getActivePage();

    zEditorContext = std::make_unique<ZEditorContext>(zDocument.get(), zWindow.get());
    zUIHandle = std::make_unique<ZUIHandle>(zEditorContext.get());
    zEditorContext->setHandle(zUIHandle.get());

    this->zWindow->setEditorContext(zEditorContext.get());
    this->zWindow->setOverlayRoot(std::make_shared<ZShape>());
    this->zWindow->setPage(page);
    this->zWindow->draw();
}

void ZApp::onUIEvent(const ZUIEvent& event) {
    if (!zUIHandle) {
        return;
    }

    zUIHandle->onUIEvent(event);
    requestRedraw();
}

void ZApp::addImage(uintptr_t ptr, size_t size, float x, float y, float width, float height) {
    if (ptr == 0 || size == 0) {
        return;
    }

    printf("ZApp::addImage\n");

    // zImages.push_back({.ptr = ptr, .size = size, .x = x, .y = y, .width = width, .height =
    // height});
}

void ZApp::clearImages() {
    zImages.clear();
}

const std::vector<ZImagePayload>& ZApp::getImages() const {
    return zImages;
}

void ZApp::requestRedraw() {
    if (!zWindow) {
        return;
    }

    zWindow->draw();
}
