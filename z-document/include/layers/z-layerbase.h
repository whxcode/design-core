
#pragma once

#include <memory>

#include "z-document/include/layers/z-component.h"
#include "z-document/include/models/z-document-model.h"
#include "z-document/include/models/z-layer-model.h"

class ZLayerBase : public ZComponent {
public:
    using ZComponent::ZComponent;
    // ZLayerBase(z_sp<ZLayerModel> model): ZComponent(model);

public:
    void setName() {};
};
