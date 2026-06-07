#include "z-paint/include/shapes/z-select-frame-shape.h"

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

void ZSelectFrameShape::setRect(const ZRect& rect) {
    zRect = rect.normalized();
}

const ZRect& ZSelectFrameShape::getRect() const {
    return zRect;
}

bool ZSelectFrameShape::getVisible() const {
    return !zRect.isEmpty();
}

void ZSelectFrameShape::render(IZEngine* engine, ZEditorContext* context) {
    if (!engine || !context || zRect.isEmpty()) {
        return;
    }

    const auto viewport = context->getViewportData();
    const auto lineWidth = viewport.scale == 0.0f ? 1.0f : 1.0f / viewport.scale;
    const auto strokeColor = ZEditorTheme::GetColor(ZEditorThemeToken::zSelectionStroke);
    const ZStyle fillStyle{
        .color = strokeColor,
        .alpha = 0.18f,
        .isStroke = false,
    };
    const ZStyle style{
        .color = strokeColor,
        .alpha = 1.0f,
        .isStroke = false,
    };

    engine->save();
    engine->transform(ZMatrix::Identity()
                          .preTranslate(viewport.offsetX, viewport.offsetY)
                          .preScale(viewport.scale, viewport.scale));

    engine->drawRect(zRect, fillStyle);
    drawFrameRect(engine, zRect, lineWidth, style);

    engine->restore();
}
