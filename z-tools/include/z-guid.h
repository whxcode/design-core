
#pragma once

#include <cstddef>
#include <string>

class ZGuid {
public:
    static ZGuid zDocumentID;
    static ZGuid zInvalidId;
    static size_t zCounter;

public:
    static ZGuid MakeGuid();

public:
    ZGuid(size_t s = 0, size_t c = 0);

public:
    bool operator!=(const ZGuid& guid) const;
    // 必须添加这个，宏才能工作
    bool operator==(const ZGuid& other) const;

public:
    const size_t toNumber() const;
    const std::string toString() const;
    const bool isInvalid();

public:
    size_t zSessionId{0};
    size_t zClientId{0};
};
