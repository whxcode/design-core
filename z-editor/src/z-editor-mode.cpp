#include "z-editor/include/z-editor-mode.h"

#include "z-app/include/ZAppEvent.h"
#include "z-editor/include/ui-event/z-ui-handle.h"
#include "z-editor/include/z-editor-context.h"
#include "z-tools/include/z-assert.h"

ZEditorMode::ZEditorMode(ZEditorContext* context) : zContext(context) {
    Z_ASSERT(zContext != nullptr, "ZEditorMode requires ZEditorContext");
}

ZEditorModeType ZEditorMode::getMode() const {
    return zMode;
}

void ZEditorMode::setMode(const ZEditorModeType mode) {
    const auto previousMode = zMode;
    zMode = mode;

    auto* handle = zContext->getHandle();
    if (!handle) {
        return;
    }

    switch (mode) {
        case ZEditorModeType::zCursor:
            handle->switchCommonHandler();
            break;
        case ZEditorModeType::zDrawRectangle:
        case ZEditorModeType::zDrawOval:
        case ZEditorModeType::zDrawVector:
            // 绘制类工具底层复用 ZDrawLayerHandle，具体绘制类型由 ZEditorMode 统一映射。
            handle->switchDrawPathHandler(toDrawLayerType(mode));
            break;
    }

    if (previousMode == zMode || !zContext->getAppEvent()) {
        return;
    }

    zContext->getAppEvent()->emit(ZAppEventType::zEditorModeChanged);
}

ZDrawLayerType ZEditorMode::toDrawLayerType(const ZEditorModeType mode) const {
    switch (mode) {
        case ZEditorModeType::zDrawRectangle:
            return ZDrawLayerType::zRectangle;
        case ZEditorModeType::zDrawOval:
            return ZDrawLayerType::zEllipse;
        case ZEditorModeType::zDrawVector:
            return ZDrawLayerType::zVector;
        case ZEditorModeType::zCursor:
            return ZDrawLayerType::zRectangle;
    }

    return ZDrawLayerType::zRectangle;
}
