#include "z-paint/include/z-skia-utils.h"

#include <algorithm>

#include "include/core/SkPathBuilder.h"

SkColor ZSkiaColor(const uint32_t color, const float alpha) {
    const auto clampedAlpha = static_cast<U8CPU>(std::clamp(alpha, 0.0f, 1.0f) * 255.0f);
    const auto red = static_cast<U8CPU>((color >> 16) & 0xFF);
    const auto green = static_cast<U8CPU>((color >> 8) & 0xFF);
    const auto blue = static_cast<U8CPU>(color & 0xFF);
    return SkColorSetARGB(clampedAlpha, red, green, blue);
}

SkMatrix ZSkiaMatrix(const ZMatrix& matrix) {
    return SkMatrix::MakeAll(matrix.get(0), matrix.get(1), matrix.get(2), matrix.get(3),
                             matrix.get(4), matrix.get(5), matrix.get(6), matrix.get(7),
                             matrix.get(8));
}

SkRect ZSkiaRect(const ZRect& rect) {
    return SkRect::MakeLTRB(rect.left(), rect.top(), rect.right(), rect.bottom());
}

SkPath ZSkiaPath(const ZPathDataArray& paths, const ZWindingRule windingRule) {
    SkPathBuilder builder;
    builder.setFillType(windingRule == ZWindingRule::kEvenOdd ? SkPathFillType::kEvenOdd
                                                               : SkPathFillType::kWinding);

    for (const auto& path : paths) {
        if (path.points.empty()) {
            continue;
        }

        const auto& firstPoint = path.points.front();
        builder.moveTo(firstPoint.point.x(), firstPoint.point.y());

        for (size_t index = 1; index < path.points.size(); ++index) {
            const auto& prevPoint = path.points[index - 1];
            const auto& currentPoint = path.points[index];

            if (prevPoint.hasCurveFrom || currentPoint.hasCurveTo) {
                const auto controlFrom =
                    prevPoint.hasCurveFrom ? prevPoint.curveFrom : prevPoint.point;
                const auto controlTo =
                    currentPoint.hasCurveTo ? currentPoint.curveTo : currentPoint.point;
                builder.cubicTo(controlFrom.x(), controlFrom.y(), controlTo.x(), controlTo.y(),
                                currentPoint.point.x(), currentPoint.point.y());
            } else {
                builder.lineTo(currentPoint.point.x(), currentPoint.point.y());
            }
        }

        if (path.isClosed) {
            builder.close();
        }
    }

    return builder.detach();
}
