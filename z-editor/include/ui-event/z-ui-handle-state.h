#pragma once

#include "z-editor/include/ui-event/z-ui-event.h"
#include "z-matrix/include/z-point.h"

class ZEditorContext;

class ZUIHandleState {
public:
    void update(const ZUIEvent& event, const ZEditorContext* context);

    ZPoint mouseDownPoint() const;
    ZPoint currentPoint() const;
    ZPoint mouseUpPoint() const;
    bool pressDown() const;

private:
    ZPoint toWorldPoint(const ZUIEvent& event, const ZEditorContext* context) const;

private:
    ZPoint zMouseDownPoint{};
    ZPoint zCurrentPoint{};
    ZPoint zMouseUpPoint{};
    bool zPressDown{false};
};
