#pragma once

#include "z-editor/include/ui-event/z-ui-handle-state.h"
#include "z-editor/include/ui-event/z-ui-event.h"
#include "z-matrix/include/z-point.h"

class ZEditorContext;
class ZCommit;
class ZDocument;
class ZSelection;
class ZTrace;
class ZUIHandle;

class ZUIHandleEvent {
public:
    friend class ZUIHandle;

    explicit ZUIHandleEvent(const ZHandlerType type, const ZUIHandleState& state,
                            ZEditorContext* context = nullptr);
    virtual ~ZUIHandleEvent() = default;

    virtual bool onUIEvent(const ZUIEvent& event);
    virtual bool onMouseDown(const ZUIEvent& event);
    virtual bool onMouseMove(const ZUIEvent& event);
    virtual bool onMouseUp(const ZUIEvent& event);
    virtual bool onMouseWheel(const ZUIEvent& event);
    virtual bool onKeyDown(const ZUIEvent& event);
    virtual bool onKeyUp(const ZUIEvent& event);

protected:
    ZEditorContext& getContext() const;
    ZDocument& getDocument() const;
    ZUIHandle& getHandle() const;
    ZSelection& getSelection() const;
    ZTrace& getTrace() const;
    ZCommit& getCommit() const;
    ZPoint getMouseDownPoint() const;
    ZPoint getCurrentPoint() const;
    ZPoint getMouseUpPoint() const;
    bool isPressDown() const;

protected:
    ZEditorContext* zContext{nullptr};
    const ZUIHandleState& zState;
    ZHandlerType zType{ZHandlerType::zCommon};
};
