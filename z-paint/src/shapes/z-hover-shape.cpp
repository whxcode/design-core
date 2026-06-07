#include "z-paint/include/shapes/z-hover-shape.h"

#include <algorithm>
#include <utility>

#include "z-document/include/viewport/z-viewport.h"
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

ZHoverShape::ZHoverShape(z_sp<ZLayerBase> layer) : zLayer(std::move(layer)) {
}

void ZHoverShape::setLayer(z_sp<ZLayerBase> layer) {
    zLayer = std::move(layer);
}

bool ZHoverShape::getVisible() const {
    return zLayer != nullptr;
}

void ZHoverShape::render(IZEngine* engine, ZEditorContext* context) {
    if (!engine || !context || !zLayer) {
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
    const auto strokeColor = ZEditorTheme::GetColor(ZEditorThemeToken::zHoverStroke);
    const ZStyle style{
        .color = strokeColor,
        .alpha = 1.0f,
        .isStroke = false,
    };

    engine->save();
    engine->transform(ZMatrix::Identity()
                          .preTranslate(viewport.offsetX, viewport.offsetY)
                          .preScale(viewport.scale, viewport.scale));

    drawFrameRect(engine, worldRect, lineWidth, style);

    engine->restore();
}
