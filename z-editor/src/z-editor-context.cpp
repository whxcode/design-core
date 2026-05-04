#include "z-editor/include/z-editor-context.h"

#include "z-app/include/ZAppEvent.h"
#include "z-document/include/layers/z-document.h"
#include "z-document/include/layers/z-page.h"
#include "z-editor/include/ui-event/z-ui-handle.h"
#include "z-window/include/ZWindow.h"

ZEditorContext::ZEditorContext(ZDocument* document, ZWindow* window)
    : zDocument(document), zWindow(window) {
}

ZDocument* ZEditorContext::getDocument() const {
    return zDocument;
}

ZWindow* ZEditorContext::getWindow() const {
    return zWindow;
}

ZUIHandle* ZEditorContext::getHandle() const {
    return zHandle;
}

z_sp<ZPage> ZEditorContext::getCurrentPage() const {
    if (!zDocument) {
        return nullptr;
    }

    return zDocument->getActivePage();
}

ZViewport* ZEditorContext::getViewport() const {
    const auto page = getCurrentPage();
    if (!page) {
        return nullptr;
    }

    return &page->getViewport();
}

ViewportData ZEditorContext::getViewportData() const {
    const auto* viewport = getViewport();
    if (!viewport) {
        return {};
    }

    return viewport->data();
}

void ZEditorContext::setViewportData(const ViewportData& data) const {
    auto* viewport = getViewport();
    if (!viewport) {
        return;
    }

    viewport->setData(data);
    if (zAppEvent) {
        zAppEvent->emit(ZAppEventType::zViewportChanged);
    }
}

void ZEditorContext::requestRedraw() const {
    if (!zWindow) {
        return;
    }

    zWindow->draw();
}

void ZEditorContext::setHandle(ZUIHandle* handle) {
    zHandle = handle;
}

void ZEditorContext::setAppEvent(ZAppEvent* appEvent) {
    zAppEvent = appEvent;
}

ZAppEvent* ZEditorContext::getAppEvent() const {
    return zAppEvent;
}
