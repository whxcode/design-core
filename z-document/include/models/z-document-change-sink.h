#pragma once

#include "z-document/include/prop/z-prop-key.h"
#include "z-document/include/prop/z-sparse-props.h"
#include "z-tools/include/z-guid.h"
#include "z-tools/include/z-type.h"

class ZModel;
class ZComponent;
class ZGuid;

class ZDocumentChangeSink {
public:
    virtual ~ZDocumentChangeSink() = default;

    virtual void onModelPropChanged(const ZGuid& id, const ZPropKey key, const PropValue& oldValue,
                                    const PropValue& newValue) = 0;
    virtual void onAddChild(const z_sp<ZComponent>& comp) = 0;
    virtual void onRemoveChild(const z_sp<ZComponent>& comp) = 0;
};
