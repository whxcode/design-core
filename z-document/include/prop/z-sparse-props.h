#pragma once

#include <future>
#include <unordered_map>

#include "z-document/include/prop/z-define-prop.h"
#include "z-document/include/prop/z-defualt-value.h"
#include "z-document/include/prop/z-prop-key.h"

using PropValue = std::variant<std::string, ZSize>;

class SparseProps {
public:
    template <ZPropKey P>
    const typename PropTraits<P>::Type& get() const {
        auto it = values->find(P);

        if (it != values->end()) {
            return std::get<typename PropTraits<P>::Type>(it->second);
        }

        return PropTraits<P>::def();
    }

    template <ZPropKey P>
    void set(const typename PropTraits<P>::Type& v) {
        if (!values) {
            values = std::make_shared<Storages>();
        }

        const auto& d = PropTraits<P>::def();

        if (d == v) {
            values->erase(P);
        } else {
            (*values)[P] = v;
        }
    }

private:
    using Storages = std::unordered_map<ZPropKey, PropValue, ZPropKeyHash>;

    std::shared_ptr<Storages> values{nullptr};
};
