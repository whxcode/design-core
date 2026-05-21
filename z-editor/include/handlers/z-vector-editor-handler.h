#pragma once

#include "z-editor/include/ui-event/z-ui-handle-event.h"

class ZVectorEditorHandler : public ZUIHandleEvent {
public:
    explicit ZVectorEditorHandler(ZHandlerType type, const ZUIHandleState& state,
                                  ZEditorContext* context = nullptr);

    bool onMouseDown(const ZUIEvent& event) override;
    bool onMouseMove(const ZUIEvent& event) override;
    bool onMouseUp(const ZUIEvent& event) override;
    bool onKeyUp(const ZUIEvent& event) override;
};
