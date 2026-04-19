#pragma once
#include <string>

#include "z-document/include/z-define-prop.h"
#include "z-document/include/z-model-type.h"
#include "z-document/include/z-prop.h"
#include "z-matrix/include/z-matrix.h"
#include "z-matrix/include/z-size.h"
#include "z-tools/include/z-guid.h"

class ZModel {
public:
    virtual ~ZModel() = default;

    /**
     * @param prop   枚举值 ZProp::zName 等
     * @param oldVal 旧值指针
     * @param newVal 新值指针
     */
    virtual void triggerUpdate(ZProp prop, void* oldVal, void* newVal);

    DEFINE_READONLY_PROP(ZGuid, Id)
    DEFINE_READONLY_PROP(ZModelType, Type)
    DEFINE_PROP(ZGuid, ParentId)
    DEFINE_PROP(std::string, Name)
    DEFINE_PROP(ZSize, Size)
    DEFINE_PROP(ZMatrix, Transform)
};
