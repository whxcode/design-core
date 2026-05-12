#pragma once

#include <any>
#include <array>
#include <future>
#include <memory>
#include <set>
#include <unordered_map>

#include "z-document/include/prop/z-default-value.h"
#include "z-document/include/prop/z-define-prop.h"
#include "z-document/include/prop/z-prop-key.h"
#include "z-matrix/include/z-matrix.h"

using PropValue = std::any;
static std::set<ZPropKey> READONLY_PROPS = {
    ZPropKey::zId,
    ZPropKey::zType,
};

class SparseProps {
public:
    using Storages = std::unordered_map<ZPropKey, PropValue, ZPropKeyHash>;

public:
    void setAny(ZPropKey key, const std::any& value) {
        if (READONLY_PROPS.contains(key)) {
            printf("!! error, try modify readonly props [%d]\n", key);
            return;
        }

        (*values)[key] = value;
    }

    const Storages& getEntry() const {
        return *values;
    }

    void merge(const SparseProps& props) {
        for (const auto& [key, value] : *props.values) {
            setAny(key, value);
        }
    }

public:
    template <ZPropKey P>
    const typename PropTraits<P>::Type& get() const {
        if (values != nullptr) {
            auto it = values->find(P);

            if (it != values->end()) {
                return std::any_cast<const typename PropTraits<P>::Type&>(it->second);
            }
        }

        return PropTraits<P>::def();
    }

    template <ZPropKey P>
    void set(const typename PropTraits<P>::Type& v) {
        if (!values) {
            values = std::make_unique<Storages>();
        }

        if (PropTraits<P>::def() == v) {
            values->erase(P);
        } else {
            (*values)[P] = v;
        }
    }

private:
    std::unique_ptr<Storages> values{nullptr};
};
