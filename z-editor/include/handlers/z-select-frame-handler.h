#pragma once

#include "z-editor/include/ui-event/z-ui-handle-event.h"
#include "z-matrix/include/z-rect.h"
#include "z-tools/include/z-guid.h"
#include "z-tools/include/z-type.h"

class ZSelectFrameShape;

class ZSelectFrameHandler : public ZUIHandleEvent {
public:
    using ZUIHandleEvent::ZUIHandleEvent;

    ~ZSelectFrameHandler() override;

    bool onMouseMove(const ZUIEvent& event) override;
    bool onMouseUp(const ZUIEvent& event) override;

private:
    void ensureFrameShape();
    void removeFrameShape();
    ZRect currentFrameRect() const;

private:
    z_sp<ZSelectFrameShape> zFrameShape{nullptr};
    ZGuid zFrameShapeId{ZGuid::zInvalidId};
};
