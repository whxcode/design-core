#include "z-paint/include/shapes/z-select-shape.h"

#include "include/core/SkCanvas.h"
#include "z-editor/include/selection/z-selection.h"
#include "z-editor/include/z-editor-context.h"
#include "z-matrix/include/z-matrix.h"
#include "z-paint/include/z-skia-utils.h"
#include "z-tools/include/z-editor-theme.h"

void ZSelectShape::render(SkCanvas* canvas, ZEditorContext* context) {
    if (!canvas || !context || !context->getSelection()) {
        return;
    }

    const auto& zRect = context->getSelection()->getSelectedLayerWorldRect();
    if (zRect.isEmpty()) {
        return;
    }

    const auto viewport = context->getViewportData();
    const auto lineWidth = viewport.scale == 0.0f ? 1.0f : 1.0f / viewport.scale;
    SkPaint paint;
    paint.setAntiAlias(true);
    paint.setStyle(SkPaint::kFill_Style);
    paint.setColor(
        ZSkiaColor(ZEditorTheme::GetColor(ZEditorThemeToken::zSelectionStroke), 1.0f));

    canvas->save();
    canvas->concat(ZSkiaMatrix(ZMatrix::Identity()
                                   .preTranslate(viewport.offsetX, viewport.offsetY)
                                   .preScale(viewport.scale, viewport.scale)));
    canvas->drawRect(SkRect::MakeXYWH(zRect.left(), zRect.top(), zRect.width(), lineWidth), paint);
    canvas->drawRect(SkRect::MakeXYWH(zRect.left(), zRect.bottom() - lineWidth, zRect.width(),
                                      lineWidth),
                     paint);
    canvas->drawRect(SkRect::MakeXYWH(zRect.left(), zRect.top(), lineWidth, zRect.height()),
                     paint);
    canvas->drawRect(SkRect::MakeXYWH(zRect.right() - lineWidth, zRect.top(), lineWidth,
                                      zRect.height()),
                     paint);
    canvas->restore();
}
