#pragma once

#include "z-editor/include/ui-event/z-ui-event.h"

class ZEditorContext;

class ZUIHandleEvent {
public:
    explicit ZUIHandleEvent(ZEditorContext* context = nullptr);
    virtual ~ZUIHandleEvent() = default;

    virtual bool onUIEvent(const ZUIEvent& event);
    virtual bool onMouseDown(const ZUIEvent& event);
    virtual bool onMouseMove(const ZUIEvent& event);
    virtual bool onMouseUp(const ZUIEvent& event);
    virtual bool onMouseWheel(const ZUIEvent& event);
    virtual bool onKeyDown(const ZUIEvent& event);
    virtual bool onKeyUp(const ZUIEvent& event);

protected:
    ZEditorContext* zContext{nullptr};
};
