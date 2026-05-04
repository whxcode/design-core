#include "z-document/include/prop/z-prop-codec.h"

#include <cstdio>
#include <string>

#include "z-matrix/include/z-matrix.h"
#include "z-matrix/include/z-size.h"
#include "z-tools/include/z-guid.h"

std::any ZPropCodec::CopyValue(const ZPropKey key, const void* value) {
    if (!value) {
        return {};
    }

    switch (key) {
        case ZPropKey::zParentId:
            return std::any(*static_cast<const ZGuid*>(value));
        case ZPropKey::zName:
            return std::any(*static_cast<const std::string*>(value));
        case ZPropKey::zSize:
            return std::any(*static_cast<const ZSize*>(value));
        case ZPropKey::zTransform:
            return std::any(*static_cast<const ZMatrix*>(value));
        case ZPropKey::zId:
        case ZPropKey::zType:
            return {};
    }

    return {};
}
