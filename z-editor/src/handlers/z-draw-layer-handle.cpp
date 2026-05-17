#include "z-editor/include/handlers/z-draw-layer-handle.h"

#include <cstdint>
#include <iostream>
#include <string>

#include "z-document/include/commit/z-commit.h"
#include "z-document/include/creator/create-model.h"
#include "z-document/include/creator/loader.h"
#include "z-document/include/layers/z-document.h"
#include "z-document/include/models/z-vector-model.h"
#include "z-editor/include/ui-event/z-ui-event.h"
#include "z-editor/include/ui-event/z-ui-handle.h"
#include "z-editor/include/z-editor-context.h"

namespace {

constexpr float kDefaultLayerX = 100.0f;
constexpr float kDefaultLayerY = 100.0f;
constexpr float kDefaultLayerW = 100.0f;
constexpr float kDefaultLayerH = 100.0f;

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

uint32_t makeFillColor(const ZDrawLayerType drawType) {
    switch (drawType) {
        case ZDrawLayerType::zRectangle:
            return 0x00ff00;
        case ZDrawLayerType::zEllipse:
            return 0x2563eb;
        case ZDrawLayerType::zVector:
            return 0xf97316;
    }

    return 0x00ff00;
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
        ZPathPoint::Make(0.0f, {118.0f, 92.0f}, ZCurveMode::kDisconnected, {118.0f, 36.0f},
                         true, true, {82.0f, 88.0f}),
        ZPathPoint::Make(0.0f, {-8.0f, 82.0f}, ZCurveMode::kDisconnected, {40.0f, 116.0f}, true,
                         true, {18.0f, 74.0f}),
    };
    return {path};
}

}  // namespace

ZDrawLayerHandle::ZDrawLayerHandle(const ZHandlerType type, const ZUIHandleState& state,
                                   ZEditorContext* context,
                                   const ZDrawLayerType drawType)
    : ZUIHandleEvent(type, state, context), zDrawType(drawType) {
}

bool ZDrawLayerHandle::onMouseDown(const ZUIEvent& event) {
    std::cout << "ZDrawPathHandle::onMouseDown" << std::endl;
    return true;
}

bool ZDrawLayerHandle::onMouseMove(const ZUIEvent& event) {
    // std::cout << "ZDrawLayerHandle::onMouseMove" << event.x << ", " << event.y << std::endl;
    return true;
}

bool ZDrawLayerHandle::onMouseUp(const ZUIEvent& event) {
    std::cout << "ZDrawPathHandle::onMouseUp" << std::endl;
    auto parent = zContext->getDocument()->getActivePage();

    auto model = ZCreatorModel::MakeLayerModel(toModelType(zDrawType));

    model->setParentId(parent->getModel()->getId());
    model->setSize({kDefaultLayerW, kDefaultLayerH});
    model->setName(makeLayerName(zDrawType));
    model->setFillColor(makeFillColor(zDrawType));
    model->setTransform(ZMatrix::Translate(kDefaultLayerX, kDefaultLayerY));

    if (zDrawType == ZDrawLayerType::zVector) {
        model->as<ZVectorModel>()->setPaths(makeDefaultVectorPaths());
    }

    const auto views = ZLoader::MakeViews({model});
    const auto layer = views.front();

    parent->addChild(layer);

    zContext->getCommit()->commit();
    zContext->getHandle()->switchCommonHandler();
    // std::cout << "MouseUp: " << event.x << ", " << event.y << std::endl;
    return true;
}

bool ZDrawLayerHandle::onMouseWheel(const ZUIEvent& event) {
    // std::cout << "MouseWheel: " << event.deltaX << ", " << event.deltaY << std::endl;
    return false;
}

bool ZDrawLayerHandle::onKeyDown(const ZUIEvent& event) {
    // std::cout << "KeyDown: " << static_cast<int>(event.keyCode) << std::endl;
    return false;
}

bool ZDrawLayerHandle::onKeyUp(const ZUIEvent& event) {
    if (event.keyCode == KeyCode::zEscape && zContext && zContext->getHandle()) {
        zContext->getHandle()->switchCommonHandler();
        return true;
    }

    return false;
}
