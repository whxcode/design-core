#pragma once

#include "z-editor/include/ui-event/z-ui-handle-event.h"

class ZUICommonHandle : public ZUIHandleEvent {
public:
    void onMouseDown(const ZUIEvent& event) override;
    void onMouseMove(const ZUIEvent& event) override;
    void onMouseUp(const ZUIEvent& event) override;
    void onMouseWheel(const ZUIEvent& event) override;
    void onKeyDown(const ZUIEvent& event) override;
    void onKeyUp(const ZUIEvent& event) override;
};
