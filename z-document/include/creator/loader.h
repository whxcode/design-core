// 将 model 转为 document-ui
#pragma once

#include "z-document/include/models/z-type.h"
#include "z-tools/include/z-type.h"

class ZDocument;

class ZLoader {
public:
    static std::unique_ptr<ZDocument> MakeDocument(ZModelArray&& models);
};
