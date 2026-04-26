#pragma once

#include "z-editor/include/ui-event/z-ui-handle-event.h"

class ZViewportHandler : public ZUIHandleEvent {
public:
    explicit ZViewportHandler(ZEditorContext* context);

    bool onMouseWheel(const ZUIEvent& event) override;

private:
    float clampScale(float scale) const;
};
