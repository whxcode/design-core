#include "z-paint/include/shapes/z-hover-shape.h"

#include <algorithm>
#include <utility>

#include "z-document/include/viewport/z-viewport.h"
#include "z-editor/include/z-editor-context.h"
#include "z-engine/include/z-engine.h"
#include "z-matrix/include/z-matrix.h"
#include "z-tools/include/z-editor-theme.h"

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

    const auto size = model->getSize();
    const auto p0 = zLayer->localToWorld(ZPoint(0.0f, 0.0f));
    const auto p1 = zLayer->localToWorld(ZPoint(size.width(), 0.0f));
    const auto p2 = zLayer->localToWorld(ZPoint(size.width(), size.height()));
    const auto p3 = zLayer->localToWorld(ZPoint(0.0f, size.height()));

    const auto left = std::min({p0.x(), p1.x(), p2.x(), p3.x()});
    const auto top = std::min({p0.y(), p1.y(), p2.y(), p3.y()});
    const auto right = std::max({p0.x(), p1.x(), p2.x(), p3.x()});
    const auto bottom = std::max({p0.y(), p1.y(), p2.y(), p3.y()});
    const auto width = right - left;
    const auto height = bottom - top;

    if (width <= 0.0f || height <= 0.0f) {
        return;
    }

    const auto viewport = context->getViewportData();
    const auto lineWidth = viewport.scale == 0.0f ? 1.0f : 1.0f / viewport.scale;
    const auto strokeColor = ZEditorTheme::GetColor(ZEditorThemeToken::zHoverStroke);
    const ZStyle style{
        .zFillColor = strokeColor,
        .zStrokeColor = strokeColor,
        .zStrokeWidth = 0.0f,
    };

    engine->save();
    engine->transform(ZMatrix::Identity()
                          .preTranslate(viewport.offsetX, viewport.offsetY)
                          .preScale(viewport.scale, viewport.scale));

    engine->drawRect(left, top, width, lineWidth, style);
    engine->drawRect(left, bottom - lineWidth, width, lineWidth, style);
    engine->drawRect(left, top, lineWidth, height, style);
    engine->drawRect(right - lineWidth, top, lineWidth, height, style);

    engine->restore();
}
