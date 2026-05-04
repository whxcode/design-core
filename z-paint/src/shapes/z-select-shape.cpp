#include "z-paint/include/shapes/z-select-shape.h"

#include "z-editor/include/selection/z-selection.h"
#include "z-editor/include/z-editor-context.h"
#include "z-engine/include/z-engine.h"
#include "z-matrix/include/z-matrix.h"
#include "z-tools/include/z-editor-theme.h"

namespace {

void drawFrameRect(IZEngine* engine, const ZRect& rect, const float lineWidth,
                   const ZStyle& style) {
    engine->drawRect(ZRect::MakeXYWH(rect.left(), rect.top(), rect.width(), lineWidth), style);
    engine->drawRect(
        ZRect::MakeXYWH(rect.left(), rect.bottom() - lineWidth, rect.width(), lineWidth), style);
    engine->drawRect(ZRect::MakeXYWH(rect.left(), rect.top(), lineWidth, rect.height()), style);
    engine->drawRect(
        ZRect::MakeXYWH(rect.right() - lineWidth, rect.top(), lineWidth, rect.height()), style);
}

}  // namespace

void ZSelectShape::render(IZEngine* engine, ZEditorContext* context) {
    const auto& zRect = context->getSelection()->getSelectedLayerWorldRect();
    if (!engine || !context || zRect.isEmpty()) {
        return;
    }

    const auto viewport = context->getViewportData();
    const auto lineWidth = viewport.scale == 0.0f ? 1.0f : 1.0f / viewport.scale;
    const auto strokeColor = ZEditorTheme::GetColor(ZEditorThemeToken::zSelectionStroke);
    const ZStyle style{
        .zFillColor = strokeColor,
        .zStrokeColor = strokeColor,
        .zStrokeWidth = 0.0f,
    };

    engine->save();
    engine->transform(ZMatrix::Identity()
                          .preTranslate(viewport.offsetX, viewport.offsetY)
                          .preScale(viewport.scale, viewport.scale));

    drawFrameRect(engine, zRect, lineWidth, style);

    engine->restore();
}
