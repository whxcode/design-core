#include "z-editor/include/handlers/z-vector-editor-handler.h"

#include "z-editor/include/ui-event/z-ui-handle.h"
#include "z-editor/include/z-editor-context.h"

ZVectorEditorHandler::ZVectorEditorHandler(const ZHandlerType type, const ZUIHandleState& state,
                                           ZEditorContext* context)
    : ZUIHandleEvent(type, state, context) {
}

bool ZVectorEditorHandler::onMouseDown(const ZUIEvent&) {
    return true;
}

bool ZVectorEditorHandler::onMouseMove(const ZUIEvent&) {
    return true;
}

bool ZVectorEditorHandler::onMouseUp(const ZUIEvent&) {
    return true;
}

bool ZVectorEditorHandler::onKeyUp(const ZUIEvent& event) {
    if (event.keyCode == KeyCode::zEscape && zContext && zContext->getHandle()) {
        zContext->getHandle()->switchCommonHandler();
        return true;
    }

    return false;
}
