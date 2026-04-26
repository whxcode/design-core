#pragma once

#include "z-editor/include/ui-event/z-ui-handle-event.h"

class ZViewportHandler : public ZUIHandleEvent {
public:
    explicit ZViewportHandler(ZEditorContext* context);

    bool onMouseDown(const ZUIEvent& event) override;
    bool onMouseMove(const ZUIEvent& event) override;
    bool onMouseUp(const ZUIEvent& event) override;
    bool onMouseWheel(const ZUIEvent& event) override;
    bool onKeyDown(const ZUIEvent& event) override;
    bool onKeyUp(const ZUIEvent& event) override;

private:
    float clampScale(float scale) const;
    float scrollDistance(float delta) const;
    float zoomFactor(float delta) const;
    void panBy(float dx, float dy) const;
    void zoomAt(float x, float y, float delta) const;

private:
    bool zSpacePressed{false};
    bool zSpacePanning{false};
    float zLastX{0.0f};
    float zLastY{0.0f};
};
