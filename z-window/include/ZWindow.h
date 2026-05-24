#pragma once

#include <functional>

#include "z-tools/include/z-type.h"
#include "z-window/include/z-surface.h"

class IZEngine;
class ZDocumentPainter;
class ZEditorContext;
class ZOverlayPainter;
class ZPage;
class ZShape;
class ZTrace;
class ZSurface;

struct WindowContext {
    size_t zWidth{0};
    size_t zHeight{0};
    size_t zPixelWidth{0};
    size_t zPixelHeight{0};
    float zDpr{0};
};

class ZWindow {
public:
    using OverlayDrawer = std::function<void(IZEngine*)>;

    ZWindow();
    void draw();
    void setContext(const WindowContext& context);
    void setPage(const z_sp<ZPage>& page);
    void setEditorContext(ZEditorContext* context);
    void setOverlayRoot(z_sp<ZShape> root);
    void setTrace(ZTrace* trace);
    void setOverlayDrawer(OverlayDrawer overlayDrawer);
    void dump() const;

private:
    void init();

private:
    int zWidth{800};
    int zHeight{800};
    float zCssWidth{800.0f};
    float zCssHeight{800.0f};
    float zDpr{0};

    IZEngine* zEngine{nullptr};
    std::shared_ptr<ZDocumentPainter> zDocumentPainter{nullptr};
    std::shared_ptr<ZOverlayPainter> zOverlayPainter{nullptr};
    ZEditorContext* zEditorContext{nullptr};
    OverlayDrawer zOverlayDrawer{nullptr};
    std::unique_ptr<ZSurface> zSurface{nullptr};
};
