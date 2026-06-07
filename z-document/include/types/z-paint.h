#pragma once

#include <cstdint>
#include <memory>
#include <vector>

struct ZPaint {
    uint32_t color{0x000000FF};
    float opacity{1.0f};
    bool visible{true};
    float strokeWidth{1.0f};  // 仅作 Strokes 时生效
};

using ZPaintArray = std::shared_ptr<std::vector<ZPaint>>;
