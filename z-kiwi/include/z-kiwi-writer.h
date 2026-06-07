#pragma once

#include "schema.h"
#include "z-document/include/models/z-type.h"

namespace kiwi {
class ByteBuffer;
}

class ZKiwiWriter {
public:
    static bool encode(const ZModelArray& models, kiwi::ByteBuffer& bb);
};
