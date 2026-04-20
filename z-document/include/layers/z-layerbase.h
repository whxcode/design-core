
#pragma once

#include <memory>

#include "z-document/include/layers/z-component.h"
#include "z-document/include/models/z-document-model.h"

class ZLayerBase : public ZComponent {
public:
    ZLayerBase(z_sp<ZDocumentModel> model);

public:
    void setName() {};
};
