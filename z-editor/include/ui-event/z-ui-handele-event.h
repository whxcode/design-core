#pragma once

#include <string>

enum class ZUIEventType {
    zMouseDown,
    zMouseMove,
    zMouseUp,
    zUnknown,
};

struct ZUIEvent {
    ZUIEventType type{ZUIEventType::zUnknown};
    float x{0.0f};
    float y{0.0f};
};

class ZUIHandleEvent {
public:
    virtual ~ZUIHandleEvent() = default;

    virtual void onUIEvent(const ZUIEvent& event);
    virtual void onMouseDown(const ZUIEvent& event) = 0;
    virtual void onMouseMove(const ZUIEvent& event) = 0;
    virtual void onMouseUp(const ZUIEvent& event) = 0;
};

class ZUICommonHandle : public ZUIHandleEvent {
public:
    void onMouseDown(const ZUIEvent& event) override;
    void onMouseMove(const ZUIEvent& event) override;
    void onMouseUp(const ZUIEvent& event) override;
};
