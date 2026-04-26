#pragma once

#include <any>
#include <future>
#include <memory>
#include <unordered_map>

#include "z-document/include/prop/z-default-value.h"
#include "z-document/include/prop/z-define-prop.h"
#include "z-document/include/prop/z-prop-key.h"
#include "z-matrix/include/z-matrix.h"

using PropValue = std::any;

class SparseProps {
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
    using Storages = std::unordered_map<ZPropKey, PropValue, ZPropKeyHash>;

    std::unique_ptr<Storages> values{nullptr};
};
