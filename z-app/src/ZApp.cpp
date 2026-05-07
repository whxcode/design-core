#include "z-app/include/ZApp.h"

#include <SDL2/SDL.h>

#include <algorithm>
#include <functional>
#include <future>
#include <iostream>
#include <random>
#include <thread>
#include <vector>

#include "z-app/include/z-test-doc.h"
#include "z-document/include/commit/z-commit.h"
#include "z-document/include/creator/loader.h"
#include "z-document/include/layers/z-layerbase.h"
#include "z-document/include/layers/z-page.h"
#include "z-document/include/models/z-layer-model.h"
#include "z-document/include/z-model-type.h"
#include "z-matrix/include/z-matrix.h"
#include "z-paint/include/z-shape.h"
#include "z-tools/include/z-task.h"
#include "z-window/include/ZWindow.h"

ZApp::ZApp() {
    // 掌握生命周期：在这里创建实例
    zAppEvent = std::make_unique<ZAppEvent>();
    zWindow = std::make_unique<ZWindow>();

    zAppEvent->on(ZAppEventType::zDocChanged, [this](const auto type) {
        this->requestRedraw();
    });
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

void ZApp::setTheme(const ZEditorThemeType type) {
    ZEditorTheme::SetTheme(type);
    requestRedraw();
}

void ZApp::startup() {
    auto futureR = DoTask([]() {
        return ZLoader::MakeDocument(ZTestDoc::MakeDoc());
    });

    zDocument = futureR.get();

    auto page = zDocument->getActivePage();

    zEditorContext = std::make_unique<ZEditorContext>(zDocument.get(), zWindow.get());
    zEditorContext->setAppEvent(zAppEvent.get());
    zSelection = std::make_unique<ZSelection>(zEditorContext.get());
    zTrace = std::make_unique<ZTrace>(zEditorContext.get(), zAppEvent.get(), [this]() {
        requestRedraw();
    });
    zUIHandle = std::make_unique<ZUIHandle>(zEditorContext.get());
    zCommit = std::make_shared<ZCommit>(zDocument, zAppEvent.get());

    zEditorContext->setHandle(zUIHandle.get());
    zEditorContext->setSelection(zSelection.get());
    zEditorContext->setTrace(zTrace.get());
    zEditorContext->setCommit(zCommit.get());

    this->zWindow->setEditorContext(zEditorContext.get());
    this->zWindow->setTrace(zTrace.get());
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
