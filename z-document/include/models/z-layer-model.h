#pragma once

#include "z-document/include/models/z-model.h"
#include "z-document/include/types/z-paint.h"
#include "z-document/include/z-model-type.h"
#include "z-tools/include/z-guid.h"

class ZLayerModel : public ZModel {
public:
    using ZModel::ZModel;

public:
    DEFINED_SPARSE_PROP(ZSize, Size)
    DEFINED_SPARSE_PROP(ZMatrix, Transform)
    DEFINED_SPARSE_PROP(ZPaintArray, Fills)
    DEFINED_SPARSE_PROP(ZPaintArray, Strokes)
};
