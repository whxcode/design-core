#include "z-editor/include/handlers/z-draw-layer-handle.h"

#include <iostream>

#include "z-document/include/commit/z-commit.h"
#include "z-document/include/creator/create-model.h"
#include "z-document/include/creator/loader.h"
#include "z-document/include/layers/z-document.h"
#include "z-editor/include/ui-event/z-ui-event.h"
#include "z-editor/include/ui-event/z-ui-handle.h"
#include "z-editor/include/z-editor-context.h"

ZDrawLayerHandle::ZDrawLayerHandle(const ZHandlerType type, ZEditorContext* context,
                                   const ZDrawLayerType drawType)
    : ZUIHandleEvent(type, context), zDrawType(drawType) {
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
    auto parent = zContext->getDocument()->getActivePage()->getFirstChild();

    auto rect1 = ZCreatorModel::Make<ZLayerModel>(ZModelType::zRectangle);
    auto rect2 = ZCreatorModel::Make<ZLayerModel>(ZModelType::zRectangle);

    rect1->setSize({50.f, 50.f});
    rect1->setName(zDrawType == ZDrawLayerType::zEllipse ? "---椭圆 1---" : "---矩形 1---");
    rect1->setFillColor(0x00ff00);
    rect1->setTransform(ZMatrix::Translate(0, 0));

    rect2->setParentId(rect1->getId());
    rect2->setSize({30.f, 30.f});
    rect2->setName("矩形 2");
    rect2->setFillColor(0xff00ff);
    rect2->setTransform(ZMatrix::Identity().preTranslate(10, 10).preRotate(12.f, 90.f, 60.f));

    const auto views = ZLoader::MakeViews({rect1, rect2});
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
