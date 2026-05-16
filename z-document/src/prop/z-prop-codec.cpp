#include "z-document/include/prop/z-prop-codec.h"

#include <cstdint>
#include <cstdio>
#include <string>
#include <vector>

#include "z-document/include/layers/z-component.h"
#include "z-document/include/models/z-layer-model.h"
#include "z-document/include/models/z-model.h"
#include "z-document/include/prop/z-prop-key.h"
#include "z-document/include/z-model-type.h"
#include "z-matrix/include/z-matrix.h"
#include "z-matrix/include/z-size.h"
#include "z-tools/include/z-guid.h"

ZPatchProps ZPropCodec::MakeProps(const z_sp<ZModel>& model) {
    ZPatchProps props{model->getProps()};

    return props;
}

void ZPropCodec::DeepBST(std::vector<z_sp<ZComponent>> comps, const Fn& fn) {
    for (auto& item : comps) {
        fn(item);
        DeepBST(item->getChildren<ZComponent>(), fn);
    }
};

void ZPropCodec::DeepBST(ZComponent* comp, const Fn& fn) {
    DeepBST({comp->as<ZComponent>()}, fn);
};
