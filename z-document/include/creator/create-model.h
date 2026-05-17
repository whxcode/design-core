#pragma once

#include <concepts>
#include <memory>

#include "z-document/include/models/z-layer-model.h"
#include "z-document/include/models/z-model.h"
#include "z-document/include/models/z-oval-model.h"
#include "z-document/include/models/z-rectangle-model.h"
#include "z-document/include/models/z-vector-model.h"
#include "z-document/include/z-model-type.h"
#include "z-tools/include/z-type.h"

class ZCreatorModel {
public:
    template <typename T>
        requires std::derived_from<T, ZModel>
    static z_sp<T> Make(const ZModelType type) {
        return std::make_shared<T>(ZGuid::MakeGuid(), type);
    };

    static z_sp<ZLayerModel> MakeLayerModel(ZModelType type);
    static z_sp<ZLayerModel> Make(const ZGuid& id, ZModelType type);
};
