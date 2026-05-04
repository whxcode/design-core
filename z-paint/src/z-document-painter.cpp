#include "z-paint/include/z-document-painter.h"

#include <functional>

#include "z-document/include/layers/z-layerbase.h"
#include "z-document/include/layers/z-page.h"
#include "z-document/include/models/z-type.h"
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
                const auto& model = layer->getModel<ZLayerModel>();
                const auto size = model->getSize();

                engine->transform(model->getTransform());
                engine->drawRect(size.width(), size.height(),
                                 {
                                     .zFillColor = model->getFillColor(),
                                 });

                render(layer->getChildren<ZLayerBase>());
                engine->restore();
            }
        };

    render(zPage->getChildren<ZLayerBase>());
    engine->restore();
}
