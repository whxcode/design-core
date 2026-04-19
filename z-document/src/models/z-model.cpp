#include "z-document/include/models/z-model.h"

#include "z-tools/include/z-guid.h"

ZModel::ZModel(ZGuid id) : zId(id) {
}

void ZModel::triggerUpdate(ZProp prop, void* oldVal, void* newVal) {
    // 这里的 prop 现在是一个数字（枚举），处理起来比字符串快得多
    // 你可以通过 switch(prop) 快速判断是哪个属性变了

    getId();
}
