#include "z-paint/include/shapes/z-select-shape.h"

#include "z-editor/include/z-editor-context.h"
#include "z-engine/include/z-engine.h"
#include "z-matrix/include/z-matrix.h"
#include "z-tools/include/z-editor-theme.h"

void ZSelectShape::setRect(const ZRect& rect) {
    zRect = rect;
}

bool ZSelectShape::getVisible() const {
    return !zRect.isEmpty();
}

void ZSelectShape::render(IZEngine* engine, ZEditorContext* context) {
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

    engine->drawRect(zRect.left(), zRect.top(), zRect.width(), lineWidth, style);
    engine->drawRect(zRect.left(), zRect.bottom() - lineWidth, zRect.width(), lineWidth, style);
    engine->drawRect(zRect.left(), zRect.top(), lineWidth, zRect.height(), style);
    engine->drawRect(zRect.right() - lineWidth, zRect.top(), lineWidth, zRect.height(), style);

    engine->restore();
}
