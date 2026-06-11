#pragma once

#include "schema.h"
#include "z-document/include/models/z-type.h"

namespace kiwi {
class ByteBuffer;
class MemoryPool;
}

class ZKiwiWriter {
public:
    static schema::Guid* getKiwiValue(kiwi::MemoryPool& pool, const ZGuid& src);
    static kiwi::Array<schema::ModelNode> encode(kiwi::MemoryPool& pool, const ZModelArray& models);
    static bool encode(const ZModelArray& models, kiwi::ByteBuffer& bb);
};
