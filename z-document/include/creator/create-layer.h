#pragma once

#include <concepts>  // 引入 concepts
#include <concepts>  // 必须包含这个
#include <memory>

#include "z-document/include/layers/z-component.h"
#include "z-document/include/layers/z-document.h"
#include "z-document/include/layers/z-layerbase.h"
#include "z-document/include/models/z-document-model.h"
#include "z-document/include/models/z-model.h"
#include "z-document/include/z-model-type.h"
#include "z-tools/include/z-type.h"

class ZCreatorLayer {
public:
    static z_sp<ZComponent> Make(const z_sp<ZModel>& model) {
        switch (model->getType()) {
            case ZModelType::zDocument:
                return std::make_shared<ZDocument>(model->as<ZDocumentModel>());

            case ZModelType::zPage:
                return std::make_shared<ZLayerBase>(model->as<ZLayerModel>());

            case ZModelType::zRectangle:
                return std::make_shared<ZLayerBase>(model->as<ZLayerModel>());
        }
    }
};
