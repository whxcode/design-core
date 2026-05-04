
#include "z-document/include/layers/z-layerbase.h"

#include <algorithm>
#include <cstdio>
#include <iostream>

#include "z-matrix/include/z-matrix.h"

// ZLayerBase::ZLayerBase(z_sp<ZDocumentModel> model) : ZComponent(model) { }

bool ZLayerBase::hitTestWorldPoint(const ZPoint& worldPoint) {
    const auto localPoint = worldToLocal(worldPoint);
    return getLocalRect().contains(localPoint);
}

ZPoint ZLayerBase::worldToLocal(const ZPoint& worldPoint) {
    const auto model = getModel<ZLayerModel>();
    if (!model) {
        return worldPoint;
    }

    ZMatrix inverse;
    if (!model->getTransform().invert(&inverse)) {
        return worldPoint;
    }

    return inverse.mapPoint(worldPoint);
}

ZPoint ZLayerBase::localToWorld(const ZPoint& localPoint) {
    const auto model = getModel<ZLayerModel>();
    if (!model) {
        return localPoint;
    }

    return model->getTransform().mapPoint(localPoint);
}

ZRect ZLayerBase::getLocalRect() {
    const auto model = getModel<ZLayerModel>();
    if (!model) {
        return ZRect::MakeEmpty();
    }

    const auto size = model->getSize();
    return ZRect::MakeXYWH(0.0f, 0.0f, size.width(), size.height());
}

ZRect ZLayerBase::getWorldRect() {
    const auto localRect = getLocalRect();
    if (localRect.isEmpty()) {
        return ZRect::MakeEmpty();
    }

    const auto p0 = localToWorld(ZPoint(localRect.left(), localRect.top()));
    const auto p1 = localToWorld(ZPoint(localRect.right(), localRect.top()));
    const auto p2 = localToWorld(ZPoint(localRect.right(), localRect.bottom()));
    const auto p3 = localToWorld(ZPoint(localRect.left(), localRect.bottom()));

    return ZRect(
        std::min({p0.x(), p1.x(), p2.x(), p3.x()}),
        std::min({p0.y(), p1.y(), p2.y(), p3.y()}),
        std::max({p0.x(), p1.x(), p2.x(), p3.x()}),
        std::max({p0.y(), p1.y(), p2.y(), p3.y()})
    );
}
