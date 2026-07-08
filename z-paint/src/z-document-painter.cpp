#include "z-paint/include/z-document-painter.h"

#include <functional>

#include "include/core/SkCanvas.h"
#include "include/core/SkPaint.h"
#include "z-document/include/layers/z-layerbase.h"
#include "z-document/include/layers/z-page.h"
#include "z-document/include/models/z-type.h"
#include "z-document/include/models/z-vector-model.h"
#include "z-matrix/include/z-matrix.h"
#include "z-paint/include/z-skia-utils.h"
#include "z-tools/include/z-assert.h"

namespace {

void drawLayerModel(SkCanvas* canvas, const z_sp<ZLayerModel>& model, const ZSize& size,
                    const SkPaint& paint) {
    switch (model->getType()) {
        case ZModelType::zRectangle:
            canvas->drawRect(SkRect::MakeWH(size.width(), size.height()), paint);
            break;
        case ZModelType::zOval:
            canvas->drawOval(SkRect::MakeWH(size.width(), size.height()), paint);
            break;
        case ZModelType::zVector: {
            const auto vectorModel = model->as<ZVectorModel>();
            if (vectorModel) {
                canvas->drawPath(
                    ZSkiaPath(vectorModel->getPaths(), vectorModel->getWindingRule()), paint);
            }
            break;
        }
        default:
            break;
    }
}

}  // namespace

void ZDocumentPainter::setPage(const z_sp<ZPage>& page) {
    zPage = page;
}

void ZDocumentPainter::draw(SkCanvas* canvas) {
    Z_ASSERT(zPage != nullptr, "error zPage is nullptr");
    Z_ASSERT(canvas != nullptr, "error canvas is nullptr");

    const auto& viewport = zPage->getViewport().data();

    canvas->save();
    canvas->concat(ZSkiaMatrix(ZMatrix::Identity()
                                   .preTranslate(viewport.offsetX, viewport.offsetY)
                                   .preScale(viewport.scale, viewport.scale)));

    std::function<void(const ZLayerBaseArray& layers)> render =
        [canvas, &render](const ZLayerBaseArray& layers) {
            for (const auto& layer : layers) {
                canvas->save();

                const auto model = layer->getModel<ZLayerModel>();
                const auto size = model->getSize();

                canvas->concat(ZSkiaMatrix(model->getTransform()));

                if (const auto fills = model->getFills()) {
                    for (const auto& fill : *fills) {
                        if (!fill.visible) continue;

                        SkPaint paint;
                        paint.setAntiAlias(true);
                        paint.setStyle(SkPaint::kFill_Style);
                        paint.setColor(ZSkiaColor(fill.color, fill.opacity));
                        drawLayerModel(canvas, model, size, paint);
                    }
                }

                if (const auto strokes = model->getStrokes()) {
                    for (const auto& stroke : *strokes) {
                        if (!stroke.visible) continue;

                        SkPaint paint;
                        paint.setAntiAlias(true);
                        paint.setStyle(SkPaint::kStroke_Style);
                        paint.setStrokeWidth(stroke.strokeWidth);
                        paint.setColor(ZSkiaColor(stroke.color, stroke.opacity));
                        drawLayerModel(canvas, model, size, paint);
                    }
                }

                render(layer->getChildren<ZLayerBase>());
                canvas->restore();
            }
        };

    render(zPage->getChildren<ZLayerBase>());
    canvas->restore();
}
