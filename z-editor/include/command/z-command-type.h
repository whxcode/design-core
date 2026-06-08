#pragma once

#include <cstdint>

enum class ZCommandType : uint8_t {
    zEscape,
    zSwitchToCommonHandler,
    zDrawRectangle,
    zDrawEllipse,
    zDrawVector,
    zUndoDocumentHistory,
    zRedoDocumentHistory,
    zDeleteSelectedLayer,
    zCancelCurrentInteraction,
    zDownloadFile,
};
