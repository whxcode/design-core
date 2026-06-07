#pragma once

#include <any>
#include <cstdint>
#include <string>

#include "z-document/include/prop/z-prop-key.h"
#include "z-document/include/types/z-path-data.h"
#include "z-document/include/types/z-paint.h"
#include "z-document/include/z-model-type.h"
#include "z-matrix/include/z-matrix.h"
#include "z-matrix/include/z-size.h"
#include "z-tools/include/z-guid.h"

template <ZPropKey p>
struct PropTraits;

/**
 * 只读属性宏：仅生成 Getter
 * 这样外部代码无法调用 set##Name，实现了编译层面的只读限制。
 */
#define DEFINE_DEFAULT_VALUE(TypeValue, PropKey)                  \
    template <>                                                   \
    struct PropTraits<ZPropKey::z##PropKey> {                     \
        using Type = TypeValue;                                   \
                                                                  \
        static const Type& def() {                                \
            static const Type defaultValue{};                     \
            return defaultValue;                                  \
        }                                                         \
                                                                  \
        static const std::any& defAny() {                         \
            static const auto defaultValueAny = std::any(Type{}); \
            return defaultValueAny;                               \
        }                                                         \
    }

DEFINE_DEFAULT_VALUE(ZGuid, Id);
DEFINE_DEFAULT_VALUE(ZGuid, ParentId);
DEFINE_DEFAULT_VALUE(std::string, Name);
DEFINE_DEFAULT_VALUE(ZModelType, Type);
DEFINE_DEFAULT_VALUE(ZSize, Size);
DEFINE_DEFAULT_VALUE(ZMatrix, Transform);
DEFINE_DEFAULT_VALUE(ZPaintArray, Fills);
DEFINE_DEFAULT_VALUE(ZPaintArray, Strokes);
DEFINE_DEFAULT_VALUE(float, StartAngle);
DEFINE_DEFAULT_VALUE(float, EndAngle);
DEFINE_DEFAULT_VALUE(float, InnerRadius);
DEFINE_DEFAULT_VALUE(ZPathDataArray, Paths);
DEFINE_DEFAULT_VALUE(ZWindingRule, WindingRule);
