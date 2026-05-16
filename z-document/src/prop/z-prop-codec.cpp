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

std::any ZPropCodec::CopyValue(const ZPropKey key, const void* value) {
    if (!value) {
        return {};
    }

    // auto a = std::any(10);

    switch (key) {
        case ZPropKey::zParentId:
            return std::any(*static_cast<const ZGuid*>(value));
        case ZPropKey::zName:
            return std::any(*static_cast<const std::string*>(value));
        case ZPropKey::zSize:
            return std::any(*static_cast<const ZSize*>(value));
        case ZPropKey::zTransform:
            return std::any(*static_cast<const ZMatrix*>(value));
        case ZPropKey::zFillColor:
            return std::any(*static_cast<const uint32_t*>(value));
        case ZPropKey::zId:
        case ZPropKey::zType:
            return {};
    }

    return {};
}

ZPatchProps ZPropCodec::MakeProps(const z_sp<ZModel>& model) {
    ZPatchProps props;
    /*

    props.set<ZPropKey::zId>(model->getId());
    props.set<ZPropKey::zType>(model->getType());
    props.set<ZPropKey::zParentId>(model->getParentId());
    props.set<ZPropKey::zName>(model->getName());

    switch (model->getType()) {
        case ZModelType::zRectangle: {
            const auto& layerModel = model->as<ZLayerModel>();

            props.set<ZPropKey::zSize>(layerModel->getSize());
            props.set<ZPropKey::zTransform>(layerModel->getTransform());
            props.set<ZPropKey::zFillColor>(layerModel->getFillColor());
        }

        default:
            break;
    }
  */

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
