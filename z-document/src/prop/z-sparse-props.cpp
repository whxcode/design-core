#pragma once

#include "z-document/include/prop/z-sparse-props.h"

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

SparseProps::SparseProps(const SparseProps& other) {
    if (other.values) {
        values = std::make_unique<Storages>(*other.values);
    }
}

SparseProps& SparseProps::operator=(const SparseProps& other) {
    if (this == &other) {
        return *this;
    }

    values = other.values ? std::make_unique<Storages>(*other.values) : nullptr;
    return *this;
}

void SparseProps::setAny(ZPropKey key, const std::any& value) {
    if (READONLY_PROPS.contains(key)) {
        // printf("!! error, try modify readonly props [%d]\n", key);
        return;
    }

    if (!values) {
        values = std::make_unique<Storages>();
    }

    (*values)[key] = value;
}

const SparseProps::Storages& SparseProps::getEntry() const {
    static const Storages empty;
    if (!values) {
        return empty;
    }

    return *values;
}

void SparseProps::merge(const SparseProps& props) {
    for (const auto& [key, value] : props.getEntry()) {
        setAny(key, value);
    }
}
