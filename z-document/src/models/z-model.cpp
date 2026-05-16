#include "z-document/include/models/z-model.h"

#include <any>
#include <string>

#include "z-document/include/models/z-document-change-sink.h"
#include "z-tools/include/z-guid.h"

ZModel::ZModel(ZGuid id, const ZModelType type) {
    initId(id);
    initType(type);
}

void ZModel::triggerUpdate(const ZPropKey prop, const PropValue& oldValue,
                           const PropValue& newValue) {
    if (!zChangeSink) {
        return;
    }

    zChangeSink->onModelPropChanged(getId(), prop, oldValue, newValue);
    // 这里的 prop 现在是一个数字（枚举），处理起来比字符串快得多
    // 你可以通过 switch(prop) 快速判断是哪个属性变了
    // getId();
}

void ZModel::setProps(const ZPatchProps& patch) {
    for (const auto& [key, value] : patch.getEntry()) {
        zProps.setAny(key, value);
    }
}

void ZModel::setPropValue(const ZPropKey key, const std::any& value) {
}
