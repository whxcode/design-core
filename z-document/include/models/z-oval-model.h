#pragma once

#include "z-document/include/models/z-layer-model.h"

class ZOvalModel : public ZLayerModel {
public:
    using ZLayerModel::ZLayerModel;

public:
    DEFINED_SPARSE_PROP(float, StartAngle)
    DEFINED_SPARSE_PROP(float, EndAngle)
    DEFINED_SPARSE_PROP(float, InnerRadius)
};
