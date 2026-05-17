#pragma once

#include <cstdint>

enum class ZCommandType : uint8_t {
    zSwitchToCommonHandler,
    zDrawRectangle,
    zDrawEllipse,
    zDrawVector,
    zUndoDocumentHistory,
    zRedoDocumentHistory,
    zDeleteSelectedLayer,
    zCancelCurrentInteraction,
};
