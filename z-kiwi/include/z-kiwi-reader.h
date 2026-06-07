#pragma once
#include "schema.h"
#include "z-document/include/models/z-type.h"

namespace kiwi {
class ByteBuffer;
}

class ZKiwiReader {
public:
    static ZModelArray decode(kiwi::ByteBuffer& bb);
};
