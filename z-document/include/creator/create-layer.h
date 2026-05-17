#pragma once

#include <concepts>
#include <memory>

#include "z-document/include/layers/z-component.h"
#include "z-document/include/layers/z-document.h"
#include "z-document/include/layers/z-layerbase.h"
#include "z-document/include/layers/z-oval-layer.h"
#include "z-document/include/layers/z-page.h"
#include "z-document/include/layers/z-rectangle-layer.h"
#include "z-document/include/layers/z-vector-layer.h"
#include "z-document/include/models/z-document-model.h"
#include "z-document/include/models/z-model.h"
#include "z-document/include/z-model-type.h"
#include "z-tools/include/z-type.h"

class ZCreatorLayer {
public:
    static z_sp<ZComponent> Make(const z_sp<ZModel>& model);
};
