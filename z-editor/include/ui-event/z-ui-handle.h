#pragma once

#include <memory>

#include "z-editor/include/ui-event/z-ui-handele-event.h"

class ZUIHandle {
public:
    ZUIHandle();

    void onUIEvent(const ZUIEvent& event);
    void setHandleEvent(std::shared_ptr<ZUIHandleEvent> handleEvent);

private:
    std::shared_ptr<ZUIHandleEvent> fZHandle{nullptr};
};
