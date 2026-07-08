#include "z-paint/include/shapes/z-hover-shape.h"

#include <utility>

#include "include/core/SkCanvas.h"
#include "z-editor/include/z-editor-context.h"
#include "z-matrix/include/z-matrix.h"
#include "z-paint/include/z-skia-utils.h"
#include "z-tools/include/z-editor-theme.h"

ZHoverShape::ZHoverShape(z_sp<ZLayerBase> layer) : zLayer(std::move(layer)) {
}

void ZHoverShape::setLayer(z_sp<ZLayerBase> layer) {
    zLayer = std::move(layer);
}

bool ZHoverShape::getVisible() const {
    return zLayer != nullptr;
}

void ZHoverShape::render(SkCanvas* canvas, ZEditorContext* context) {
    if (!canvas || !context || !zLayer) {
        return;
    }

    const auto model = zLayer->getModel<ZLayerModel>();
    if (!model) {
        return;
    }

    const auto worldRect = zLayer->getWorldRect();
    if (worldRect.isEmpty()) {
        return;
    }

    const auto viewport = context->getViewportData();
    const auto lineWidth = viewport.scale == 0.0f ? 1.0f : 1.0f / viewport.scale;
    SkPaint paint;
    paint.setAntiAlias(true);
    paint.setStyle(SkPaint::kFill_Style);
    paint.setColor(ZSkiaColor(ZEditorTheme::GetColor(ZEditorThemeToken::zHoverStroke), 1.0f));

    canvas->save();
    canvas->concat(ZSkiaMatrix(ZMatrix::Identity()
                                   .preTranslate(viewport.offsetX, viewport.offsetY)
                                   .preScale(viewport.scale, viewport.scale)));
    canvas->drawRect(SkRect::MakeXYWH(worldRect.left(), worldRect.top(), worldRect.width(),
                                      lineWidth),
                     paint);
    canvas->drawRect(SkRect::MakeXYWH(worldRect.left(), worldRect.bottom() - lineWidth,
                                      worldRect.width(), lineWidth),
                     paint);
    canvas->drawRect(SkRect::MakeXYWH(worldRect.left(), worldRect.top(), lineWidth,
                                      worldRect.height()),
                     paint);
    canvas->drawRect(SkRect::MakeXYWH(worldRect.right() - lineWidth, worldRect.top(), lineWidth,
                                      worldRect.height()),
                     paint);
    canvas->restore();
}
