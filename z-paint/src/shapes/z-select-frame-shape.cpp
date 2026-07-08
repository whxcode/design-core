#include "z-paint/include/shapes/z-select-frame-shape.h"

#include "include/core/SkCanvas.h"
#include "z-editor/include/z-editor-context.h"
#include "z-matrix/include/z-matrix.h"
#include "z-paint/include/z-skia-utils.h"
#include "z-tools/include/z-editor-theme.h"

void ZSelectFrameShape::setRect(const ZRect& rect) {
    zRect = rect.normalized();
}

const ZRect& ZSelectFrameShape::getRect() const {
    return zRect;
}

bool ZSelectFrameShape::getVisible() const {
    return !zRect.isEmpty();
}

void ZSelectFrameShape::render(SkCanvas* canvas, ZEditorContext* context) {
    if (!canvas || !context || zRect.isEmpty()) {
        return;
    }

    const auto viewport = context->getViewportData();
    const auto lineWidth = viewport.scale == 0.0f ? 1.0f : 1.0f / viewport.scale;
    const auto strokeColor = ZEditorTheme::GetColor(ZEditorThemeToken::zSelectionStroke);
    SkPaint fillPaint;
    fillPaint.setAntiAlias(true);
    fillPaint.setStyle(SkPaint::kFill_Style);
    fillPaint.setColor(ZSkiaColor(strokeColor, 0.18f));

    SkPaint strokePaint;
    strokePaint.setAntiAlias(true);
    strokePaint.setStyle(SkPaint::kFill_Style);
    strokePaint.setColor(ZSkiaColor(strokeColor, 1.0f));

    canvas->save();
    canvas->concat(ZSkiaMatrix(ZMatrix::Identity()
                                   .preTranslate(viewport.offsetX, viewport.offsetY)
                                   .preScale(viewport.scale, viewport.scale)));
    canvas->drawRect(ZSkiaRect(zRect), fillPaint);
    canvas->drawRect(SkRect::MakeXYWH(zRect.left(), zRect.top(), zRect.width(), lineWidth),
                     strokePaint);
    canvas->drawRect(SkRect::MakeXYWH(zRect.left(), zRect.bottom() - lineWidth, zRect.width(),
                                      lineWidth),
                     strokePaint);
    canvas->drawRect(SkRect::MakeXYWH(zRect.left(), zRect.top(), lineWidth, zRect.height()),
                     strokePaint);
    canvas->drawRect(SkRect::MakeXYWH(zRect.right() - lineWidth, zRect.top(), lineWidth,
                                      zRect.height()),
                     strokePaint);
    canvas->restore();
}
