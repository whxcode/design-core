#include "z-editor/include/handlers/z-draw-layer-handle.h"

#include <cmath>
#include <string>

#include "z-document/include/commit/z-commit.h"
#include "z-document/include/creator/create-model.h"
#include "z-document/include/creator/loader.h"
#include "z-document/include/layers/z-document.h"
#include "z-document/include/layers/z-layerbase.h"
#include "z-document/include/layers/z-page.h"
#include "z-document/include/models/z-layer-model.h"
#include "z-document/include/models/z-vector-model.h"
#include "z-editor/include/selection/z-selection.h"
#include "z-editor/include/ui-event/z-ui-event.h"
#include "z-editor/include/ui-event/z-ui-handle.h"
#include "z-editor/include/z-editor-context.h"
#include "z-matrix/include/z-matrix.h"

namespace {

constexpr float kMinDrawingSize = 0.0f;

ZModelType toModelType(const ZDrawLayerType drawType) {
    switch (drawType) {
        case ZDrawLayerType::zRectangle:
            return ZModelType::zRectangle;
        case ZDrawLayerType::zEllipse:
            return ZModelType::zOval;
        case ZDrawLayerType::zVector:
            return ZModelType::zVector;
    }

    return ZModelType::zRectangle;
}

std::string makeLayerName(const ZDrawLayerType drawType) {
    switch (drawType) {
        case ZDrawLayerType::zRectangle:
            return "矩形";
        case ZDrawLayerType::zEllipse:
            return "椭圆";
        case ZDrawLayerType::zVector:
            return "向量";
    }

    return "图层";
}

ZPaintArray makeFills(const ZDrawLayerType drawType) {
    auto fills = std::make_shared<std::vector<ZPaint>>();
    switch (drawType) {
        case ZDrawLayerType::zRectangle:
            fills->push_back({0x00ff00, 0.5f, true});  // 底色黑
            break;
        case ZDrawLayerType::zEllipse:
            fills->push_back({0x33ff00, 0.5f, true});  // 底色黑
            break;
        case ZDrawLayerType::zVector:
            fills->push_back({0xaaff44, 0.5f, true});  // 底色黑
            break;
    }

    return fills;
}

ZPathDataArray makeDefaultVectorPaths() {
    ZPathData path;
    path.isClosed = true;

    // 默认向量使用不规则闭合曲线，方便肉眼验证 cubic bezier 的出入控制点是否生效。
    path.points = {
        ZPathPoint::Make(0.0f, {28.0f, -18.0f}, ZCurveMode::kDisconnected, {10.0f, 0.0f}, true,
                         false, {10.0f, 18.0f}),
        ZPathPoint::Make(0.0f, {108.0f, 18.0f}, ZCurveMode::kDisconnected, {56.0f, -8.0f}, true,
                         true, {86.0f, 14.0f}),
        ZPathPoint::Make(0.0f, {118.0f, 92.0f}, ZCurveMode::kDisconnected, {118.0f, 36.0f}, true,
                         true, {82.0f, 88.0f}),
        ZPathPoint::Make(0.0f, {-8.0f, 82.0f}, ZCurveMode::kDisconnected, {40.0f, 116.0f}, true,
                         true, {18.0f, 74.0f}),
    };
    return {path};
}

float directionScale(const float delta) {
    return delta < 0.0f ? -1.0f : 1.0f;
}

}  // namespace

ZDrawLayerHandle::ZDrawLayerHandle(const ZHandlerType type, const ZUIHandleState& state,
                                   ZEditorContext* context, const ZDrawLayerType drawType)
    : ZUIHandleEvent(type, state, context), zDrawType(drawType) {
}

bool ZDrawLayerHandle::onMouseDown(const ZUIEvent& event) {
    printf("ZDrawLayerHandle::onMouseDown\n");
    if (event.button != MouseButton::zLeft) {
        return false;
    }

    cancelDrawingLayer();
    zDrawingLayer = createDrawingLayer();
    updateDrawingLayer(getMouseDownPoint());

    return true;
}

bool ZDrawLayerHandle::onMouseMove(const ZUIEvent&) {
    if (!zDrawingLayer) {
        return false;
    }

    updateDrawingLayer(getCurrentPoint());

    return true;
}

bool ZDrawLayerHandle::onMouseUp(const ZUIEvent&) {
    if (!zDrawingLayer) {
        getHandle().switchCommonHandler();
        return false;
    }

    updateDrawingLayer(getMouseUpPoint());

    const auto model = zDrawingLayer->getModel<ZLayerModel>();
    const auto size = model->getSize();
    if (size.width() <= kMinDrawingSize || size.height() <= kMinDrawingSize) {
        cancelDrawingLayer();
        getHandle().switchCommonHandler();
        return true;
    }

    zDrawingParent = nullptr;
    zDrawingLayer = nullptr;

    getCommit().commit();
    getHandle().switchCommonHandler();
    return true;
}

bool ZDrawLayerHandle::onMouseWheel(const ZUIEvent&) {
    return false;
}

bool ZDrawLayerHandle::onKeyDown(const ZUIEvent&) {
    return false;
}

bool ZDrawLayerHandle::onKeyUp(const ZUIEvent& event) {
    if (event.keyCode == KeyCode::zEscape && zContext && zContext->getHandle()) {
        cancelDrawingLayer();
        zContext->getHandle()->switchCommonHandler();
        return true;
    }

    return false;
}

z_sp<ZLayerBase> ZDrawLayerHandle::createDrawingLayer() {
    zDrawingParent = getDocument().getActivePage();
    if (!zDrawingParent) {
        return nullptr;
    }

    const auto model = ZCreatorModel::MakeLayerModel(toModelType(zDrawType));
    model->setParentId(zDrawingParent->getModel()->getId());
    model->setSize(ZSize::MakeEmpty());
    model->setName(makeLayerName(zDrawType));
    model->setFills(makeFills(zDrawType));
    model->setTransform(ZMatrix::Translate(getMouseDownPoint().x(), getMouseDownPoint().y()));

    if (zDrawType == ZDrawLayerType::zVector) {
        model->as<ZVectorModel>()->setPaths(makeDefaultVectorPaths());
    }

    printf("model[%d]\n", model.get());
    const auto views = ZLoader::MakeViews({model});
    if (views.empty()) {
        return nullptr;
    }

    const auto layer = views.front();
    zDrawingParent->addChild(layer);
    getSelection().select(layer);

    return layer;
}

void ZDrawLayerHandle::updateDrawingLayer(const ZPoint& point) {
    if (!zDrawingLayer) {
        return;
    }

    const auto startPoint = getMouseDownPoint();
    const float dx{point.x() - startPoint.x()};
    const float dy{point.y() - startPoint.y()};
    const float width{std::abs(dx)};
    const float height{std::abs(dy)};

    auto model = zDrawingLayer->getModel<ZLayerModel>();
    model->setSize(ZSize::Make(width, height));
    model->setTransform(ZMatrix::Translate(startPoint.x(), startPoint.y())
                            .preScale(directionScale(dx), directionScale(dy)));
}

void ZDrawLayerHandle::cancelDrawingLayer() {
    if (zDrawingParent && zDrawingLayer) {
        zDrawingParent->removeChild(zDrawingLayer);
    }

    zDrawingParent = nullptr;
    zDrawingLayer = nullptr;
}
