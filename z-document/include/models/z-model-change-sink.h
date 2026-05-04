#pragma once

#include "z-document/include/prop/z-prop-key.h"
class ZModel;

class ZModelChangeSink {
public:
    virtual ~ZModelChangeSink() = default;

    virtual void onModelPropChanged(const ZModel* const model, const ZPropKey key,
                                    const void* const oldValue, const void* const newValue) = 0;
};
