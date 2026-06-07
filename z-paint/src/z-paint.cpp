#include "z-paint/include/z-paint.h"

#include <functional>

#include "z-document/include/layers/z-component.h"
#include "z-document/include/layers/z-layerbase.h"
#include "z-document/include/models/z-type.h"
#include "z-document/include/models/z-vector-model.h"
#include "z-engine/include/z-engine.h"
#include "z-tools/include/z-assert.h"

namespace {

void drawType(IZEngine* engine, ZModelType type, const ZSize& size, const ZStyle& style,
              const z_sp<ZVectorModel>& vectorModel) {
    switch (type) {
        case ZModelType::zRectangle:
            engine->drawRect(size.width(), size.height(), style);
            break;
        case ZModelType::zOval:
            engine->drawOval(size.width(), size.height(), style);
            break;
        case ZModelType::zVector:
            if (vectorModel) {
                engine->drawPath(vectorModel->getPaths(), vectorModel->getWindingRule(), style);
            }
            break;
        default:
            break;
    }
}

}  // namespace

ZPainterBase::ZPainterBase(IZEngine* engine) : zEngine(engine) {
}

void ZPainterBase::draw() {
    std::function<void(const ZLayerBaseArray& layers)> render =
        [this, &render](const ZLayerBaseArray& layers) {
            for (auto layer : layers) {
                zEngine->save();
                const auto model = layer->getModel<ZLayerModel>();
                const auto size = model->getSize();

                zEngine->transform(model->getTransform());

                // 绘制 fills
                if (auto fills = model->getFills()) {
                    for (const auto& paint : *fills) {
                        if (!paint.visible) continue;
                        ZStyle style;
                        style.color = paint.color;
                        style.alpha = paint.opacity;
                        style.isStroke = false;
                        drawType(zEngine, model->getType(), size, style,
                                 model->as<ZVectorModel>());
                    }
                }

                // 绘制 strokes
                if (auto strokes = model->getStrokes()) {
                    for (const auto& paint : *strokes) {
                        if (!paint.visible) continue;
                        ZStyle style;
                        style.color = paint.color;
                        style.alpha = paint.opacity;
                        style.isStroke = true;
                        style.strokeWidth = paint.strokeWidth;
                        drawType(zEngine, model->getType(), size, style,
                                 model->as<ZVectorModel>());
                    }
                }

                render(layer->getChildren<ZLayerBase>());
                zEngine->restore();
            }
        };

    Z_ASSERT(fComponent != nullptr, "error fComponent is nullptr");
    Z_ASSERT(zEngine != nullptr, "error zEngine is nullptr");

    render(fComponent->getChildren<ZLayerBase>());
}

void ZPainterBase::setComponent(const z_sp<ZLayerBase>& comp) {
    fComponent = comp;
}
