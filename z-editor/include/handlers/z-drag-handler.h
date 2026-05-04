#pragma once

#include "z-editor/include/ui-event/z-ui-handle-event.h"
#include "z-matrix/include/z-point.h"

class ZDragHandler : public ZUIHandleEvent {
public:
    using ZUIHandleEvent::ZUIHandleEvent;

    void setLastPoint(const ZPoint& point);
    bool onMouseMove(const ZUIEvent& event) override;
    bool onMouseUp(const ZUIEvent& event) override;

private:
    ZPoint zLastPoint{};
};
