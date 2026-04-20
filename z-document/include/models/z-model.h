#pragma once
#include <memory>
#include <string>

#include "z-document/include/z-define-prop.h"
#include "z-document/include/z-model-type.h"
#include "z-document/include/z-prop.h"
#include "z-matrix/include/z-matrix.h"
#include "z-matrix/include/z-size.h"
#include "z-tools/include/z-guid.h"
#include "z-tools/include/z-type.h"

class ZModel : public std::enable_shared_from_this<ZModel> {
public:
    ZModel(ZGuid id, const ZModelType type);

    virtual ~ZModel() = default;

public:
    template <typename T>
        requires std::derived_from<T, ZModel>
    z_sp<T> as() {
        // shared_from_this() 保证了引用计数是同步的
        // static_pointer_cast 负责安全的向下转型
        return std::static_pointer_cast<T>(shared_from_this());
    }

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
};
