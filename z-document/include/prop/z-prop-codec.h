#pragma once

#include <any>

#include "z-document/include/prop/z-prop-key.h"

class ZPropCodec {
public:
    static std::any CopyValue(ZPropKey key, const void* value);
};
