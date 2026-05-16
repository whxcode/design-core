#pragma once

#include <cstdint>

enum class ZCommandType : uint8_t {
    zSwitchToCommonHandler,
    zDrawRectangle,
    zDrawEllipse,
    zUndoDocumentHistory,
    zRedoDocumentHistory,
    zDeleteSelectedLayer,
    zCancelCurrentInteraction,
};
