#pragma once
#include <utility>

/**
 * Type: 属性类型 (如 ZGuid)
 * Name: 核心标识符 (如 Id)
 * * 展开逻辑：
 * - 变量名: zId
 * - 方法名: setId / getId
 * - 枚举名: ZProp::zId
 */

/**
 * 只读属性宏：仅生成 Getter
 * 这样外部代码无法调用 set##Name，实现了编译层面的只读限制。
 */
#define DEFINE_READONLY_PROP(Type, Name) \
private:                                 \
    const Type z##Name{};                \
                                         \
public:                                  \
    const Type& get##Name() const {      \
        return this->z##Name;            \
    }                                    \
    /* 注意：这里不提供 setter */

#define DEFINE_PROP(Type, Name)                                                       \
private:                                                                              \
    Type z##Name{};                                                                   \
                                                                                      \
public:                                                                               \
    /* Setter: 左值 */                                                                \
    void set##Name(const Type& newValue) {                                            \
        if (this->z##Name == newValue) return;                                        \
        Type oldValue = this->z##Name;                                                \
        this->z##Name = newValue;                                                     \
        /* 触发更新，传递枚举 ZProp::zId */                                           \
        this->triggerUpdate(ZProp::z##Name, (void*)&oldValue, (void*)&this->z##Name); \
    }                                                                                 \
    /* Setter: 右值 */                                                                \
    void set##Name(Type&& newValue) {                                                 \
        if (this->z##Name == newValue) return;                                        \
        Type oldValue = std::move(this->z##Name);                                     \
        this->z##Name = std::move(newValue);                                          \
        this->triggerUpdate(ZProp::z##Name, (void*)&oldValue, (void*)&this->z##Name); \
    }                                                                                 \
    /* Getters */                                                                     \
    const Type& get##Name() const {                                                   \
        return this->z##Name;                                                         \
    }                                                                                 \
    Type& get##Name() {                                                               \
        return this->z##Name;                                                         \
    }
