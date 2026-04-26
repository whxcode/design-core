#include "z-editor/include/ui-event/z-ui-handle.h"

#include <utility>

#include "z-editor/include/ui-event/z-ui-common-handle.h"

ZUIHandle::ZUIHandle() : fZHandle(std::make_shared<ZUICommonHandle>()) {
}

void ZUIHandle::onUIEvent(const ZUIEvent& event) {
    if (!fZHandle) {
        return;
    }

    fZHandle->onUIEvent(event);
}

void ZUIHandle::setHandleEvent(std::shared_ptr<ZUIHandleEvent> handleEvent) {
    fZHandle = std::move(handleEvent);
}
