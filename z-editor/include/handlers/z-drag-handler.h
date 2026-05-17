#pragma once

#include "z-editor/include/ui-event/z-ui-handle-event.h"
#include "z-matrix/include/z-point.h"

class ZDragHandler : public ZUIHandleEvent {
public:
    explicit ZDragHandler(ZHandlerType type, const ZUIHandleState& state,
                          ZEditorContext* context = nullptr);

    bool onMouseMove(const ZUIEvent& event) override;
    bool onMouseUp(const ZUIEvent& event) override;

private:
    ZPoint zLastPoint{};
};
