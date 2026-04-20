#pragma once

#include "z-document/include/models/z-model.h"
#include "z-document/include/z-model-type.h"
#include "z-tools/include/z-guid.h"

class ZDocumentModel : public ZModel {
public:
    ZDocumentModel(ZGuid _id, const ZModelType _type)
        : ZModel(ZGuid::zDocumentID, ZModelType::zDocument) {
        setParentId(ZGuid::zInvalidId);
    }
};
