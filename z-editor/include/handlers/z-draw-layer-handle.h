#pragma once

#include "z-editor/include/ui-event/z-ui-handle-event.h"

class ZDrawLayerHandle : public ZUIHandleEvent {
public:
    explicit ZDrawLayerHandle(ZHandlerType type, const ZUIHandleState& state,
                              ZEditorContext* context = nullptr,
                              ZDrawLayerType drawType = ZDrawLayerType::zRectangle);

    bool onMouseDown(const ZUIEvent& event) override;
    bool onMouseMove(const ZUIEvent& event) override;
    bool onMouseUp(const ZUIEvent& event) override;
    bool onMouseWheel(const ZUIEvent& event) override;
    bool onKeyDown(const ZUIEvent& event) override;
    bool onKeyUp(const ZUIEvent& event) override;

private:
    ZDrawLayerType zDrawType{ZDrawLayerType::zRectangle};
};
