#pragma once

#include "z-editor/include/ui-event/z-ui-handle-event.h"

class ZUICommonHandle : public ZUIHandleEvent {
public:
    using ZUIHandleEvent::ZUIHandleEvent;

    bool onMouseDown(const ZUIEvent& event) override;
    bool onMouseMove(const ZUIEvent& event) override;
    bool onMouseUp(const ZUIEvent& event) override;
    bool onMouseWheel(const ZUIEvent& event) override;
    bool onKeyDown(const ZUIEvent& event) override;
    bool onKeyUp(const ZUIEvent& event) override;

private:
    bool zDragPending{false};
};
