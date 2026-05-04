#include "z-document/include/models/z-layer-model.h"

#include <any>

void ZLayerModel::setPropValue(const ZPropKey key, const std::any& value) {
    switch (key) {
        case ZPropKey::zSize:
            setSize(std::any_cast<const ZSize&>(value));
            return;
        case ZPropKey::zTransform:
            setTransform(std::any_cast<const ZMatrix&>(value));
            return;
        case ZPropKey::zId:
        case ZPropKey::zType:
        case ZPropKey::zParentId:
        case ZPropKey::zName:
            ZModel::setPropValue(key, value);
            return;
    }
}
