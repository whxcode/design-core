#include "z-paint/include/z-paint.h"

#include <functional>
#include <iostream>

#include "z-document/include/layers/z-component.h"
#include "z-document/include/layers/z-layerbase.h"
#include "z-document/include/models/z-type.h"
#include "z-engine/include/z-engine.h"
#include "z-tools/include/z-assert.h"

ZPaint::ZPaint(IZEngine* engine) : zEngine(engine) {
}

void ZPaint::draw() {
    std::function<void(const ZLayerBaseArray& layers)> render =
        [this, &render](const ZLayerBaseArray& layers) {
            for (auto layer : layers) {
                // std::cout << "layer:" << layer->getModel()->getName() << std::endl;

                zEngine->save();
                const auto& model = layer->getModel<ZLayerModel>();

                auto size = model->getSize();

                zEngine->transform(model->getTransform());
                zEngine->drawRect(size.width(), size.height(),
                                  {
                                      .zFillColor = model->getFillColor(),

                                  });

                render(layer->getChildren<ZLayerBase>());

                zEngine->restore();
            }
        };

    Z_ASSERT(fComponent != nullptr, "error fComponent is nullptr");
    Z_ASSERT(zEngine != nullptr, "error zEngine is nullptr");

    render(fComponent->getChildren<ZLayerBase>());
}

void ZPaint::setComponent(const z_sp<ZLayerBase>& comp) {
    fComponent = comp;
}
