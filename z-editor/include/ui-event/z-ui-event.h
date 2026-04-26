#pragma once

#include <cstdint>

enum class ZUIEventType {
    zMouseDown,
    zMouseMove,
    zMouseUp,
    zMouseWheel,
    zKeyDown,
    zKeyUp,
    zUnknown,
};

enum class KeyCode : uint8_t {
    zUnknown,
    zA,
    zB,
    zC,
    zD,
    zE,
    zF,
    zG,
    zH,
    zI,
    zJ,
    zK,
    zL,
    zM,
    zN,
    zO,
    zP,
    zQ,
    zR,
    zS,
    zT,
    zU,
    zV,
    zW,
    zX,
    zY,
    zZ,
    zDigit0,
    zDigit1,
    zDigit2,
    zDigit3,
    zDigit4,
    zDigit5,
    zDigit6,
    zDigit7,
    zDigit8,
    zDigit9,
    zEscape,
    zEnter,
    zSpace,
    zBackspace,
    zTab,
    zShift,
    zControl,
    zAlt,
    zMeta,
    zArrowLeft,
    zArrowRight,
    zArrowUp,
    zArrowDown,
    zDelete,
};

struct ZUIEvent {
    ZUIEventType type{ZUIEventType::zUnknown};
    float x{0.0f};
    float y{0.0f};
    float deltaX{0.0f};
    float deltaY{0.0f};
    KeyCode keyCode{KeyCode::zUnknown};
    bool metaKey{false};
    bool ctrlKey{false};
    bool altKey{false};
    bool shiftKey{false};
};
