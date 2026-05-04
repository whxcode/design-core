#pragma once

#include "z-editor/include/ui-event/z-ui-event.h"
#include "z-matrix/include/z-point.h"

class ZEditorContext;

class ZUIHandleEvent {
public:
    friend class ZUIHandle;

    explicit ZUIHandleEvent(const ZHandlerType type, ZEditorContext* context = nullptr);
    virtual ~ZUIHandleEvent() = default;

    virtual bool onUIEvent(const ZUIEvent& event);
    virtual bool onMouseDown(const ZUIEvent& event);
    virtual bool onMouseMove(const ZUIEvent& event);
    virtual bool onMouseUp(const ZUIEvent& event);
    virtual bool onMouseWheel(const ZUIEvent& event);
    virtual bool onKeyDown(const ZUIEvent& event);
    virtual bool onKeyUp(const ZUIEvent& event);

protected:
    ZPoint getMouseDownPoint() const;
    ZPoint getCurrentPoint() const;
    ZPoint getMouseUpPoint() const;
    ZPoint toWorldPoint(const ZUIEvent& event) const;

protected:
    ZEditorContext* zContext{nullptr};
    ZHandlerType zType{ZHandlerType::zCommon};
    ZPoint zMouseDownPoint{};
    ZPoint zCurrentPoint{};
    ZPoint zMouseUpPoint{};
};
