#pragma once

#include "z-editor/include/ui-event/z-ui-event.h"

class ZUIHandleEvent {
public:
    virtual ~ZUIHandleEvent() = default;

    virtual void onUIEvent(const ZUIEvent& event);
    virtual void onMouseDown(const ZUIEvent& event) = 0;
    virtual void onMouseMove(const ZUIEvent& event) = 0;
    virtual void onMouseUp(const ZUIEvent& event) = 0;
    virtual void onMouseWheel(const ZUIEvent& event) = 0;
    virtual void onKeyDown(const ZUIEvent& event) = 0;
    virtual void onKeyUp(const ZUIEvent& event) = 0;
};
