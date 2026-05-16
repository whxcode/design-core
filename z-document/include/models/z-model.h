#pragma once
#include <memory>
#include <string>

#include "z-document/include/commit/z-patch.h"
#include "z-document/include/prop/z-define-prop.h"
#include "z-document/include/prop/z-prop-key.h"
#include "z-document/include/prop/z-sparse-props.h"
#include "z-document/include/z-model-type.h"
#include "z-matrix/include/z-matrix.h"
#include "z-matrix/include/z-size.h"
#include "z-tools/include/z-guid.h"
#include "z-tools/include/z-type.h"

class ZDocumentChangeSink;

class ZModel : public std::enable_shared_from_this<ZModel> {
public:
    ZModel(ZGuid id, const ZModelType type);

    virtual ~ZModel() = default;
    // 删除默认赋值运算符
    ZModel& operator=(const ZModel&) = delete;
    ZModel& operator=(ZModel&&) = delete;  // 可选：同时删除移动赋值

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
    virtual void triggerUpdate(const ZPropKey prop, const PropValue& oldValue,
                               const PropValue& newValue);

    DEFINED_SPARSE_PROP_READONLY(ZGuid, Id)
    DEFINED_SPARSE_PROP_READONLY(ZModelType, Type)

    DEFINED_SPARSE_PROP(ZGuid, ParentId)
    DEFINED_SPARSE_PROP(std::string, Name)

public:
    void setChangeSink(ZDocumentChangeSink* sink) {
        zChangeSink = sink;
    }

    ZDocumentChangeSink* getChangeSink() const {
        return zChangeSink;
    }

    void merge(const SparseProps& props) {
        zProps.merge(props);
    }

    SparseProps getProps() {
        return zProps;
    }

    void setProps(const ZPatchProps& props);

protected:
    virtual void setPropValue(ZPropKey key, const std::any& value);

    template <ZPropKey P>
    const typename PropTraits<P>::Type& getProp() const {
        return zProps.get<P>();
    }

    template <ZPropKey P>
    void setProp(const typename PropTraits<P>::Type& value) {
        const auto& oldVal = zProps.get<P>();

        if (oldVal == value) {
            return;
        }

        auto oldValue = oldVal;
        auto newValue = std::any(value);

        zProps.set<P>(value);

        triggerUpdate(P, oldValue, newValue);
    }

    ZDocumentChangeSink* zChangeSink{nullptr};

private:
    SparseProps zProps;
};
