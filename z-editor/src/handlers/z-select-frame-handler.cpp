#include "z-editor/include/handlers/z-select-frame-handler.h"

#include "z-editor/include/selection/z-selection.h"
#include "z-editor/include/ui-event/z-ui-handle.h"
#include "z-editor/include/z-editor-context.h"
#include "z-paint/include/shapes/z-select-frame-shape.h"
#include "z-paint/include/z-trace.h"

ZSelectFrameHandler::~ZSelectFrameHandler() {
    removeFrameShape();
}

bool ZSelectFrameHandler::onMouseMove(const ZUIEvent&) {
    if (!zContext || !zContext->getTrace()) {
        return true;
    }

    ensureFrameShape();
    zFrameShape->setRect(currentFrameRect());
    if (zContext->getSelection()) {
        zContext->getSelection()->selectInRect(zFrameShape->getRect());
    }
    zContext->requestRedraw();
    return true;
}

bool ZSelectFrameHandler::onMouseUp(const ZUIEvent&) {
    if (zContext && zContext->getTrace()) {
        ensureFrameShape();
        zFrameShape->setRect(currentFrameRect());
    }

    removeFrameShape();

    if (zContext && zContext->getHandle()) {
        zContext->getHandle()->switchCommonHandler();
    }

    if (zContext) {
        zContext->requestRedraw();
    }

    return true;
}

void ZSelectFrameHandler::ensureFrameShape() {
    if (zFrameShape || !zContext || !zContext->getTrace()) {
        return;
    }

    zFrameShape = zContext->getTrace()->MakeShape<ZSelectFrameShape>();
    zFrameShapeId = zContext->getTrace()->addChild(zFrameShape);
}

void ZSelectFrameHandler::removeFrameShape() {
    if (!zContext || !zContext->getTrace() || zFrameShapeId == ZGuid::zInvalidId) {
        return;
    }

    zContext->getTrace()->removeChild(zFrameShapeId);
    zFrameShapeId = ZGuid::zInvalidId;
    zFrameShape = nullptr;
}

ZRect ZSelectFrameHandler::currentFrameRect() const {
    return ZRect::MakeLTRB(
        getMouseDownPoint().x(),
        getMouseDownPoint().y(),
        getCurrentPoint().x(),
        getCurrentPoint().y()
    );
}
