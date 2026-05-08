
#pragma once

#include <memory>

#include "z-document/include/layers/z-component.h"
#include "z-document/include/models/z-document-model.h"
#include "z-document/include/models/z-layer-model.h"
#include "z-matrix/include/z-matrix.h"
#include "z-matrix/include/z-point.h"
#include "z-matrix/include/z-rect.h"

class ZLayerBase : public ZComponent {
public:
    using ZComponent::ZComponent;
    // ZLayerBase(z_sp<ZLayerModel> model): ZComponent(model);

public:
    bool hitTestWorldPoint(const ZPoint& worldPoint);
    z_sp<ZLayerBase> getFirstChild() const;
    ZPoint worldToLocal(const ZPoint& worldPoint);
    ZPoint localToWorld(const ZPoint& localPoint);
    ZMatrix getWorldMatrix();
    ZMatrix getParentWorldMatrix();
    ZMatrix getParentInvertWorldMatrix();
    ZRect getLocalRect();
    ZRect getWorldRect();

    void setName() {};
};
