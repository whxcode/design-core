#pragma once

#include "z-editor/include/ui-event/z-ui-handle-event.h"
#include "z-tools/include/z-type.h"

class ZLayerBase;
class ZPage;

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
    z_sp<ZLayerBase> createDrawingLayer();
    void updateDrawingLayer(const ZPoint& point);
    void cancelDrawingLayer();

private:
    ZDrawLayerType zDrawType{ZDrawLayerType::zRectangle};
    z_sp<ZPage> zDrawingParent{nullptr};
    z_sp<ZLayerBase> zDrawingLayer{nullptr};
};
