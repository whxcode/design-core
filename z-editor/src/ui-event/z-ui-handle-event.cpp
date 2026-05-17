#include "z-editor/include/ui-event/z-ui-handle-event.h"

#include "z-document/include/commit/z-commit.h"
#include "z-document/include/layers/z-document.h"
#include "z-editor/include/selection/z-selection.h"
#include "z-editor/include/ui-event/z-ui-handle.h"
#include "z-editor/include/z-editor-context.h"
#include "z-paint/include/z-trace.h"
#include "z-tools/include/z-assert.h"

ZUIHandleEvent::ZUIHandleEvent(const ZHandlerType type, const ZUIHandleState& state,
                               ZEditorContext* context)
    : zContext(context), zState(state), zType(type) {
}

bool ZUIHandleEvent::onUIEvent(const ZUIEvent& event) {
    switch (event.type) {
        case ZUIEventType::zMouseDown:
            return onMouseDown(event);
        case ZUIEventType::zMouseMove:
            return onMouseMove(event);
        case ZUIEventType::zMouseUp:
            return onMouseUp(event);
        case ZUIEventType::zMouseWheel:
            return onMouseWheel(event);
        case ZUIEventType::zKeyDown:
            return onKeyDown(event);
        case ZUIEventType::zKeyUp:
            return onKeyUp(event);
        case ZUIEventType::zUnknown:
            break;
    }

    return false;
}

bool ZUIHandleEvent::onMouseDown(const ZUIEvent&) {
    return false;
}

bool ZUIHandleEvent::onMouseMove(const ZUIEvent&) {
    return false;
}

bool ZUIHandleEvent::onMouseUp(const ZUIEvent&) {
    return false;
}

bool ZUIHandleEvent::onMouseWheel(const ZUIEvent&) {
    return false;
}

bool ZUIHandleEvent::onKeyDown(const ZUIEvent&) {
    return false;
}

bool ZUIHandleEvent::onKeyUp(const ZUIEvent&) {
    return false;
}

ZEditorContext& ZUIHandleEvent::getContext() const {
    Z_ASSERT(zContext != nullptr, "ZUIHandleEvent requires ZEditorContext");
    return *zContext;
}

ZDocument& ZUIHandleEvent::getDocument() const {
    auto* document = getContext().getDocument();
    Z_ASSERT(document != nullptr, "ZUIHandleEvent requires ZDocument");
    return *document;
}

ZUIHandle& ZUIHandleEvent::getHandle() const {
    auto* handle = getContext().getHandle();
    Z_ASSERT(handle != nullptr, "ZUIHandleEvent requires ZUIHandle");
    return *handle;
}

ZSelection& ZUIHandleEvent::getSelection() const {
    auto* selection = getContext().getSelection();
    Z_ASSERT(selection != nullptr, "ZUIHandleEvent requires ZSelection");
    return *selection;
}

ZTrace& ZUIHandleEvent::getTrace() const {
    auto* trace = getContext().getTrace();
    Z_ASSERT(trace != nullptr, "ZUIHandleEvent requires ZTrace");
    return *trace;
}

ZCommit& ZUIHandleEvent::getCommit() const {
    auto* commit = getContext().getCommit();
    Z_ASSERT(commit != nullptr, "ZUIHandleEvent requires ZCommit");
    return *commit;
}

ZPoint ZUIHandleEvent::getMouseDownPoint() const {
    return zState.mouseDownPoint();
}

ZPoint ZUIHandleEvent::getCurrentPoint() const {
    return zState.currentPoint();
}

ZPoint ZUIHandleEvent::getMouseUpPoint() const {
    return zState.mouseUpPoint();
}

bool ZUIHandleEvent::isPressDown() const {
    return zState.pressDown();
}
