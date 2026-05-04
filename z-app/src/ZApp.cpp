#include "z-app/include/ZApp.h"

#include <SDL2/SDL.h>

#include <functional>
#include <future>
#include <iostream>
#include <random>
#include <thread>
#include <vector>

#include "z-app/include/z-test-doc.h"
#include "z-document/include/commit/z-commit.h"
#include "z-document/include/creator/loader.h"
#include "z-document/include/models/z-layer-model.h"
#include "z-matrix/include/z-matrix.h"
#include "z-paint/include/z-shape.h"
#include "z-tools/include/z-task.h"
#include "z-window/include/ZWindow.h"

ZApp::ZApp() {
    // 掌握生命周期：在这里创建实例
    zAppEvent = std::make_unique<ZAppEvent>();
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

ZAppEvent& ZApp::getAppEvent() const {
    return *zAppEvent;
}

void ZApp::startup() {
    auto futureR = DoTask([]() {
        return ZLoader::MakeDocument(ZTestDoc::MakeDoc());
    });

    zDocument = futureR.get();

    auto page = zDocument->getActivePage();

    zEditorContext = std::make_unique<ZEditorContext>(zDocument.get(), zWindow.get());
    zEditorContext->setAppEvent(zAppEvent.get());
    zUIHandle = std::make_unique<ZUIHandle>(zEditorContext.get());
    zCommit = std::make_shared<ZCommit>(zDocument, zAppEvent.get());

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

ZCommit& ZApp::getCommit() const {
    return *zCommit;
}

void ZApp::requestRedraw() {
    if (!zWindow) {
        return;
    }

    zWindow->draw();
}

void ZApp::randProps() {
    if (!zDocument) {
        return;
    }

    const auto layers = zDocument->getNonPageLayers();
    if (layers.empty()) {
        return;
    }

    static std::mt19937 rng{std::random_device{}()};
    std::uniform_int_distribution<size_t> pickLayer(0, layers.size() - 1);
    std::uniform_real_distribution<float> moveDelta(-80.0f, 80.0f);
    std::uniform_real_distribution<float> rotateDelta(-25.0f, 25.0f);

    const auto& layer = layers[pickLayer(rng)];
    const auto model = layer->getModel<ZLayerModel>();
    if (!model) {
        return;
    }

    auto next = model->getTransform();
    next.preTranslate(moveDelta(rng), moveDelta(rng));
    next.preRotate(rotateDelta(rng));
    model->setTransform(next);

    zCommit->commit();

    requestRedraw();
}
