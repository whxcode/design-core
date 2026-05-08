#pragma once

#include "z-document/include/prop/z-prop-key.h"
#include "z-tools/include/z-type.h"

class ZModel;
class ZComponent;

class ZDocumentChangeSink {
public:
    virtual ~ZDocumentChangeSink() = default;

    virtual void onModelPropChanged(const ZModel* const model, const ZPropKey key,
                                    const void* const oldValue, const void* const newValue) = 0;
    virtual void onAddChild(const z_sp<ZComponent>& comp) = 0;
    virtual void onRemoveChild(const z_sp<ZComponent>& comp) = 0;
};
