#include "z-document/include/creator/create-layer.h"

z_sp<ZComponent> ZCreatorLayer::Make(const z_sp<ZModel>& model) {
    if (!model) {
        return nullptr;
    }

    switch (model->getType()) {
        case ZModelType::zDocument:
            return std::make_shared<ZDocument>(model->as<ZDocumentModel>());
        case ZModelType::zPage:
            return std::make_shared<ZPage>(model->as<ZLayerModel>());
        case ZModelType::zRectangle:
            return std::make_shared<ZRectangleLayer>(model->as<ZLayerModel>());
        case ZModelType::zOval:
            return std::make_shared<ZOvalLayer>(model->as<ZLayerModel>());
        case ZModelType::zVector:
            return std::make_shared<ZVectorLayer>(model->as<ZLayerModel>());
    }

    return nullptr;
}
