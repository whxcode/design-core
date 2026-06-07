#include "z-paint/include/z-document-painter.h"

#include <functional>

#include "z-document/include/layers/z-layerbase.h"
#include "z-document/include/layers/z-page.h"
#include "z-document/include/models/z-type.h"
#include "z-document/include/models/z-vector-model.h"
#include "z-engine/include/z-engine.h"
#include "z-matrix/include/z-matrix.h"
#include "z-tools/include/z-assert.h"

void ZDocumentPainter::setPage(const z_sp<ZPage>& page) {
    zPage = page;
}

void ZDocumentPainter::draw(IZEngine* engine) {
    Z_ASSERT(zPage != nullptr, "error zPage is nullptr");
    Z_ASSERT(engine != nullptr, "error engine is nullptr");

    const auto& viewport = zPage->getViewport().data();

    engine->save();
    engine->transform(ZMatrix::Identity()
                          .preTranslate(viewport.offsetX, viewport.offsetY)
                          .preScale(viewport.scale, viewport.scale));

    std::function<void(const ZLayerBaseArray& layers)> render =
        [&engine, &render](const ZLayerBaseArray& layers) {
            for (auto layer : layers) {
                engine->save();
                const auto model = layer->getModel<ZLayerModel>();
                const auto size = model->getSize();

                engine->transform(model->getTransform());

                // 先画所有 fills（填充）
                if (auto fills = model->getFills()) {
                    for (const auto& paint : *fills) {
                        if (!paint.visible) continue;

                        ZStyle style;
                        style.color = paint.color;
                        style.alpha = paint.opacity;
                        style.isStroke = false;

                        switch (model->getType()) {
                            case ZModelType::zRectangle:
                                engine->drawRect(size.width(), size.height(), style);
                                break;
                            case ZModelType::zOval:
                                engine->drawOval(size.width(), size.height(), style);
                                break;
                            case ZModelType::zVector: {
                                const auto vectorModel = model->as<ZVectorModel>();
                                engine->drawPath(vectorModel->getPaths(),
                                                 vectorModel->getWindingRule(), style);
                                break;
                            }
                            default:
                                break;
                        }
                    }
                }

                // 再画所有 strokes（描边，叠在 fills 之上）
                if (auto strokes = model->getStrokes()) {
                    for (const auto& paint : *strokes) {
                        if (!paint.visible) continue;

                        ZStyle style;
                        style.color = paint.color;
                        style.alpha = paint.opacity;
                        style.isStroke = true;
                        style.strokeWidth = paint.strokeWidth;

                        switch (model->getType()) {
                            case ZModelType::zRectangle:
                                engine->drawRect(size.width(), size.height(), style);
                                break;
                            case ZModelType::zOval:
                                engine->drawOval(size.width(), size.height(), style);
                                break;
                            case ZModelType::zVector: {
                                const auto vectorModel = model->as<ZVectorModel>();
                                engine->drawPath(vectorModel->getPaths(),
                                                 vectorModel->getWindingRule(), style);
                                break;
                            }
                            default:
                                break;
                        }
                    }
                }

                render(layer->getChildren<ZLayerBase>());
                engine->restore();
            }
        };

    render(zPage->getChildren<ZLayerBase>());
    engine->restore();
}
