
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

z_sp<ZLayerBase> ZLayerBase::getFirstChild() const {
    const auto children = getChildren<ZLayerBase>();
    if (children.empty()) {
        return nullptr;
    }

    return children.front();
}

ZPoint ZLayerBase::worldToLocal(const ZPoint& worldPoint) {
    ZMatrix inverse;
    if (!getWorldMatrix().invert(&inverse)) {
        return worldPoint;
    }

    return inverse.mapPoint(worldPoint);
}

ZPoint ZLayerBase::localToWorld(const ZPoint& localPoint) {
    return getWorldMatrix().mapPoint(localPoint);
}

ZMatrix ZLayerBase::getWorldMatrix() {
    const auto model = getModel<ZLayerModel>();
    if (!model) {
        return ZMatrix::Identity();
    }

    auto matrix = model->getTransform();
    auto parent = getParent();
    while (parent && parent->getType() != ZModelType::zDocument) {
        const auto parentLayer = parent->as<ZLayerBase>();
        matrix.postConcat(parentLayer->getModel<ZLayerModel>()->getTransform());
        parent = parent->getParent();
    }

    return matrix;
}

ZMatrix ZLayerBase::getParentWorldMatrix() {
    auto parent = getParent();
    if (!parent || parent->getType() == ZModelType::zDocument) {
        return ZMatrix::Identity();
    }

    return parent->as<ZLayerBase>()->getWorldMatrix();
}

ZMatrix ZLayerBase::getParentInvertWorldMatrix() {
    ZMatrix inverse;
    if (!getParentWorldMatrix().invert(&inverse)) {
        return ZMatrix::Identity();
    }

    return inverse;
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
    return getWorldMatrix().mapRect(getLocalRect());
}
