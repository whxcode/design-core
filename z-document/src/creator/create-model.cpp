#include "z-document/include/creator/create-model.h"

z_sp<ZLayerModel> ZCreatorModel::MakeLayerModel(const ZModelType type) {
    return Make(ZGuid::MakeGuid(), type);
}

z_sp<ZLayerModel> ZCreatorModel::Make(const ZGuid& id, const ZModelType type) {
    switch (type) {
        case ZModelType::zRectangle:
            return std::make_shared<ZRectangleModel>(id, type);
        case ZModelType::zOval:
            return std::make_shared<ZOvalModel>(id, type);
        case ZModelType::zVector:
            return std::make_shared<ZVectorModel>(id, type);
        case ZModelType::zDocument:
        case ZModelType::zPage:
            return std::make_shared<ZLayerModel>(id, type);
    }

    return nullptr;
}
