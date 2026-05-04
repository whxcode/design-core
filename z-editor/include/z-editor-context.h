#pragma once

#include "z-document/include/viewport/z-viewport.h"
#include "z-tools/include/z-type.h"

class ZDocument;
class ZPage;
class ZSelection;
class ZTrace;
class ZUIHandle;
class ZWindow;
class ZAppEvent;

class ZEditorContext {
public:
    ZEditorContext(ZDocument* document, ZWindow* window);

    ZDocument* getDocument() const;
    ZWindow* getWindow() const;
    ZUIHandle* getHandle() const;
    z_sp<ZPage> getCurrentPage() const;
    ZViewport* getViewport() const;
    ViewportData getViewportData() const;
    void setViewportData(const ViewportData& data) const;
    void requestRedraw() const;
    void setHandle(ZUIHandle* handle);
    void setAppEvent(ZAppEvent* appEvent);
    ZAppEvent* getAppEvent() const;
    void setSelection(ZSelection* selection);
    ZSelection* getSelection() const;
    void setTrace(ZTrace* trace);
    ZTrace* getTrace() const;

private:
    ZDocument* zDocument{nullptr};
    ZWindow* zWindow{nullptr};
    ZUIHandle* zHandle{nullptr};
    ZAppEvent* zAppEvent{nullptr};
    ZSelection* zSelection{nullptr};
    ZTrace* zTrace{nullptr};
};
