#pragma once

#include "z-document/include/models/z-model.h"
#include "z-tools/include/z-type.h"

class ZComponent {
public:
    ZComponent(z_sp<ZModel> model);

private:
    z_sp<ZModel> zModel{nullptr};
};
