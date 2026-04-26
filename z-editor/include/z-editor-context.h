#pragma once

#include "z-document/include/viewport/z-viewport.h"
#include "z-tools/include/z-type.h"

class ZDocument;
class ZPage;
class ZWindow;

class ZEditorContext {
public:
    ZEditorContext(ZDocument* document, ZWindow* window);

    ZDocument* getDocument() const;
    ZWindow* getWindow() const;
    z_sp<ZPage> getCurrentPage() const;
    ZViewport* getViewport() const;
    ViewportData getViewportData() const;
    void setViewportData(const ViewportData& data) const;
    void requestRedraw() const;

private:
    ZDocument* zDocument{nullptr};
    ZWindow* zWindow{nullptr};
};
