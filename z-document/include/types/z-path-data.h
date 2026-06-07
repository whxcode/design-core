#pragma once

#include <cstdint>
#include <vector>

#include "z-matrix/include/z-point.h"

enum class ZCurveMode : uint8_t {
    kStraight = 1,
    kAsymmetric = 3,
    kDisconnected = 4,
    kMirrored = 5,
};

enum class ZWindingRule : uint8_t {
    kEvenOdd = 0,
    kNonZero = 1,
};

struct ZPathPoint {
    float cornerRadius{0.0f};
    ZPoint curveFrom{};
    ZPoint curveTo{};
    ZPoint point{};
    bool hasCurveFrom{false};
    bool hasCurveTo{false};
    bool fixed{false};
    ZCurveMode curveMode{ZCurveMode::kStraight};

    static ZPathPoint Make(const ZPoint& point, const float cornerRadius = 0.0f) {
        ZPathPoint pathPoint;
        pathPoint.cornerRadius = cornerRadius;
        pathPoint.curveFrom = point;
        pathPoint.curveTo = point;
        pathPoint.point = point;
        return pathPoint;
    }

    static ZPathPoint Make(float cornerRadius, const ZPoint& curveFrom, ZCurveMode curveMode,
                           const ZPoint& curveTo, bool hasCurveFrom, bool hasCurveTo,
                           const ZPoint& point) {
        ZPathPoint pathPoint;
        pathPoint.cornerRadius = cornerRadius;
        pathPoint.curveFrom = curveFrom;
        pathPoint.curveMode = curveMode;
        pathPoint.curveTo = curveTo;
        pathPoint.hasCurveFrom = hasCurveFrom;
        pathPoint.hasCurveTo = hasCurveTo;
        pathPoint.point = point;
        return pathPoint;
    }

    friend bool operator==(const ZPathPoint& left, const ZPathPoint& right) {
        return left.cornerRadius == right.cornerRadius &&  //
               left.curveFrom == right.curveFrom &&        //
               left.curveTo == right.curveTo &&            //
               left.point == right.point &&                //
               left.hasCurveFrom == right.hasCurveFrom &&  //
               left.hasCurveTo == right.hasCurveTo &&      //
               left.fixed == right.fixed &&                //
               left.curveMode == right.curveMode;
    }

    friend bool operator!=(const ZPathPoint& left, const ZPathPoint& right) {
        return !(left == right);
    }
};

struct ZPathData {
    std::vector<ZPathPoint> points{};
    bool isClosed{true};

    friend bool operator==(const ZPathData& left, const ZPathData& right) {
        return left.points == right.points && left.isClosed == right.isClosed;
    }

    friend bool operator!=(const ZPathData& left, const ZPathData& right) {
        return !(left == right);
    }
};

using ZPathDataArray = std::vector<ZPathData>;
