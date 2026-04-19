#pragma once
#include <string>

#include "z-tools/include/z-guid.h"

class ZModel {
public:
    ZGuid zGuid;
    std::string zName{};  // 图层名称。
};
