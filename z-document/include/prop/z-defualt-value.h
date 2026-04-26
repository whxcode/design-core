#pragma once

#include "z-document/include/prop/z-prop-key.h"
#include "z-matrix/include/z-size.h"

template <ZPropKey p>
struct PropTraits;

/**
 * 只读属性宏：仅生成 Getter
 * 这样外部代码无法调用 set##Name，实现了编译层面的只读限制。
 */
#define DEFINE_DEFAULT_VALUE(TypeValue, PropKey) \
    template <>                                  \
    struct PropTraits<ZPropKey::z##PropKey> {    \
        using Type = TypeValue;                  \
                                                 \
        static const Type& def() {               \
            static const Type defaultValue{};    \
            return defaultValue;                 \
        }                                        \
    }

DEFINE_DEFAULT_VALUE(ZSize, Size);
