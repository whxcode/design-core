#include "z-tools/include/z-guid.h"

#include <cstddef>

size_t ZGuid::zCounter{5};
ZGuid ZGuid::zInvalidId{0, 0};
ZGuid ZGuid::zDocumentID{0, 1};

ZGuid ZGuid::MakeGuid() {
    return ZGuid{++zCounter, 0};
}

ZGuid::ZGuid(size_t s, size_t c) : zSessionId(s), zClientId(c) {
}

bool ZGuid::operator!=(const ZGuid& guid) const {
    return zSessionId != guid.zSessionId || zClientId != guid.zClientId;
}

bool ZGuid::operator==(const ZGuid& other) const {
    return !(*this != other);
}

const size_t ZGuid::toNumber() const {
    return zSessionId + zClientId;
}

const std::string ZGuid::toString() const {
    return std::to_string(zSessionId) + std::to_string(zClientId);
}

const bool ZGuid::isInvalid() {
    return !(*this != zInvalidId);
}
