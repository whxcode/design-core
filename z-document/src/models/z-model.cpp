#include "z-document/include/models/z-model.h"

#include <any>
#include <string>

#include "z-document/include/models/z-document-change-sink.h"
#include "z-tools/include/z-guid.h"

ZModel::ZModel(ZGuid id, const ZModelType type) : zId(id), zType(type) {
    // printf("any [%d\n]", sizeof(std::any));
    // printf("std::varinat[%d\n]", sizeof(PropValue));
}

void ZModel::triggerUpdate(ZPropKey prop, void* oldVal, void* newVal) {
    if (!zChangeSink) {
        return;
    }

    zChangeSink->onModelPropChanged(this, prop, oldVal, newVal);
    // 这里的 prop 现在是一个数字（枚举），处理起来比字符串快得多
    // 你可以通过 switch(prop) 快速判断是哪个属性变了
    // getId();
}

void ZModel::setProps(const ZPatchProps& props) {
    for (const auto& [key, value] : props) {
        setPropValue(key, value);
    }
}

void ZModel::setPropValue(const ZPropKey key, const std::any& value) {
    switch (key) {
        case ZPropKey::zParentId:
            setParentId(std::any_cast<const ZGuid&>(value));
            return;
        case ZPropKey::zName:
            setName(std::any_cast<const std::string&>(value));
            return;
        case ZPropKey::zId:
        case ZPropKey::zType:
        case ZPropKey::zSize:
        case ZPropKey::zTransform:
        case ZPropKey::zFillColor:
            return;
    }
}
