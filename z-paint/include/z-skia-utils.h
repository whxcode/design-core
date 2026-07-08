#pragma once

#include <cstdint>

#include "include/core/SkColor.h"
#include "include/core/SkMatrix.h"
#include "include/core/SkPath.h"
#include "include/core/SkRect.h"
#include "z-document/include/types/z-path-data.h"
#include "z-matrix/include/z-matrix.h"
#include "z-matrix/include/z-rect.h"

SkColor ZSkiaColor(uint32_t color, float alpha);
SkMatrix ZSkiaMatrix(const ZMatrix& matrix);
SkRect ZSkiaRect(const ZRect& rect);
SkPath ZSkiaPath(const ZPathDataArray& paths, ZWindingRule windingRule);
