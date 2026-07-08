#pragma once

#include <memory>

#include "z-tools/include/z-type.h"

class ZDocumentPainter;
class ZEditorContext;
class ZOverlayPainter;
class ZPage;
class ZShape;
class ZTrace;
struct ZWindowSkiaState;

struct WindowContext {
    size_t zWidth{0};
    size_t zHeight{0};
    size_t zPixelWidth{0};
    size_t zPixelHeight{0};
    float zDpr{0};
};

class ZWindow {
public:
    ZWindow();
    ~ZWindow();
    void draw();
    void setContext(const WindowContext& context);
    void setPage(const z_sp<ZPage>& page);
    void setEditorContext(ZEditorContext* context);
    void setOverlayRoot(z_sp<ZShape> root);
    void setTrace(ZTrace* trace);
    void dump() const;

private:
    void init();
    void ensureSurface();
    void destroySurface();
    void present();

private:
    int zWidth{800};
    int zHeight{800};
    float zCssWidth{800.0f};
    float zCssHeight{800.0f};
    float zDpr{1.0f};

    std::unique_ptr<ZWindowSkiaState> zSkia{nullptr};
    std::shared_ptr<ZDocumentPainter> zDocumentPainter{nullptr};
    std::shared_ptr<ZOverlayPainter> zOverlayPainter{nullptr};
    ZEditorContext* zEditorContext{nullptr};
};
