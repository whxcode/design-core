
#pragma once

#include <cstddef>
#include <string>

class ZGuid {
public:
    static ZGuid zDocumentID;
    static ZGuid zInvalid;
    static size_t zCounter;

public:
    static ZGuid MakeGuid();

private:
    ZGuid(size_t s, size_t c);

public:
    bool operator!=(const ZGuid& guid);

public:
    const size_t toNumber() const;
    const std::string toString() const;
    const bool isInvalid();

public:
    size_t zSessionId{0};
    size_t zClientId{0};
};
