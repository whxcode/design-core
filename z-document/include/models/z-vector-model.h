#pragma once

#include "z-document/include/models/z-layer-model.h"
#include "z-document/include/types/z-path-data.h"

class ZVectorModel : public ZLayerModel {
public:
    using ZLayerModel::ZLayerModel;

public:
    DEFINED_SPARSE_PROP(ZPathDataArray, Paths)
    DEFINED_SPARSE_PROP(ZWindingRule, WindingRule)
};
