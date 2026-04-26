#pragma once

#include "z-document/include/layers/z-layerbase.h"
#include "z-document/include/viewport/z-viewport.h"

class ZPage : public ZLayerBase {
public:
    using ZLayerBase::ZLayerBase;

    ZViewport& getViewport();
    const ZViewport& getViewport() const;

private:
    ZViewport zViewport;
};
