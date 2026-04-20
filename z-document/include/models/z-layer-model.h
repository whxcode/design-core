#pragma once

#include "z-document/include/models/z-model.h"
#include "z-document/include/z-model-type.h"
#include "z-tools/include/z-guid.h"

class ZLayerModel : public ZModel {
public:
    using ZModel::ZModel;

    DEFINE_PROP(ZSize, Size)
    DEFINE_PROP(ZMatrix, Transform)
};
