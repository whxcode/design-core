#pragma once

#include "z-document/include/prop/z-prop-key.h"

class ZModel;
class ZComponent;

class ZDocumentChangeSink {
public:
    virtual ~ZDocumentChangeSink() = default;

    virtual void onModelPropChanged(const ZModel* const model, const ZPropKey key,
                                    const void* const oldValue, const void* const newValue) = 0;
    virtual void onAddChild(ZComponent* comp) = 0;
    virtual void onRemoveChild(ZComponent* comp) = 0;
};
