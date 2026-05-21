#pragma once

#include <cstdint>

#include "z-editor/include/ui-event/z-ui-event.h"

class ZEditorContext;

enum class ZEditorModeType : uint8_t {
    zCursor,
    zDrawRectangle,
    zDrawOval,
    zDrawVector,
};

class ZEditorMode {
public:
    explicit ZEditorMode(ZEditorContext* context);

    ZEditorModeType getMode() const;
    void setMode(ZEditorModeType mode);

private:
    ZDrawLayerType toDrawLayerType(ZEditorModeType mode) const;

private:
    ZEditorContext* zContext{nullptr};
    ZEditorModeType zMode{ZEditorModeType::zCursor};
};
