#pragma once

#include <memory>
#include <ranges>
#include <vector>

#include "z-document/include/models/z-model.h"
#include "z-tools/include/z-type.h"

class ZComponent : public std::enable_shared_from_this<ZComponent> {
public:
    ZComponent(z_sp<ZModel> model);

public:
    template <typename T>
        requires std::derived_from<T, ZComponent>
    z_sp<T> as() {
        // shared_from_this() 保证了引用计数是同步的
        // static_pointer_cast 负责安全的向下转型
        return std::static_pointer_cast<T>(shared_from_this());
    }

    template <typename T>
        requires std::derived_from<T, ZModel>
    z_sp<T> getModel() {
        return zModel->as<T>();
    }

    template <typename T>
        requires std::derived_from<T, ZModel>
    z_sp<T> getModel() const {
        return zModel->as<T>();
    }

    z_sp<ZModel> getModel() {
        return getModel<ZModel>();
    }

    z_sp<ZModel> getModel() const {
        return getModel<ZModel>();
    }

    template <typename T>
        requires std::derived_from<T, ZComponent>
    std::vector<z_sp<T>> getChildren() {
        std::vector<z_sp<T>> result;
        for (const auto& comp : zLayers) {
            result.push_back(comp->as<T>());
        }
        return result;
    }

    template <typename T>
        requires std::derived_from<T, ZComponent>
    std::vector<z_sp<T>> getChildren() const {
        std::vector<z_sp<T>> result;
        for (const auto& comp : zLayers) {
            result.push_back(comp->as<T>());
        }
        return result;
    }

public:
    virtual void addChild(const z_sp<ZComponent>& comp);
    virtual void removeChild(const z_sp<ZComponent>& comp);
    z_sp<ZComponent> getParent() const;

public:
    inline size_t getUnique() const {
        return zRuntimeId;
    };

    inline ZModelType getType() const {
        return zModel->getType();
    };

    inline size_t getParentUnique() const {
        return zParentRuntimeId;
    };

private:
    const z_sp<ZModel> zModel{nullptr};
    std::vector<z_sp<ZComponent>> zLayers{};
    std::weak_ptr<ZComponent> zParent{};
    size_t zRuntimeId{0};
    size_t zParentRuntimeId{0};
};
