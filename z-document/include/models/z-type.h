#pragma once

#include <vector>

#include "z-document/include/layers/z-component.h"
#include "z-document/include/layers/z-layerbase.h"
#include "z-document/include/models/z-model.h"
#include "z-tools/include/z-type.h"

using ZModelArray = std::vector<z_sp<ZModel>>;
using ZComponentArray = std::vector<z_sp<ZComponent>>;
using ZLayerBaseArray = std::vector<z_sp<ZLayerBase>>;
