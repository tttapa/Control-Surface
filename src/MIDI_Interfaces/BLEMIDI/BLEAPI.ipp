#pragma once

#include "BLEAPI.hpp"

#include <cassert>
#include <functional>
#include <utility>

#include <AH/Error/Error.hpp>

BEGIN_CS_NAMESPACE

struct BLEDataGenerator::Iface {
    virtual ~Iface() = default;
    virtual BLEDataView next() = 0;
    virtual Iface *move_into(void *storage) noexcept = 0;

    Iface(const Iface &) = delete;
    Iface &operator=(const Iface &) = delete;

  protected:
    Iface() = default;
    Iface(Iface &&) = default;
};

template <class T>
struct BLEDataGenerator::Impl : Iface {
    T inst;
    template <class... Args>
    Impl(Args &&...args) : inst {std::forward<Args>(args)...} {}
    BLEDataView next() override { return std::invoke(inst); }
    Iface *move_into(void *storage) noexcept override {
        return new (storage) Impl {std::move(*this)};
    }
};

inline BLEDataView BLEDataGenerator::operator()() {
    assert(instance);
    return instance->next();
}

inline void BLEDataGenerator::clear() {
    if (auto *inst = std::exchange(instance, nullptr))
        inst->~Iface();
}

template <class T, class... Args>
BLEDataGenerator::BLEDataGenerator(std::in_place_type_t<T>, Args &&...args) {
    static_assert(sizeof(Impl<T>) <= sizeof(storage));
    static_assert(alignof(Impl<T>) <= alignof(buffer_align_t));
    instance = new (storage) Impl<T> {std::forward<Args>(args)...};
}

template <class T>
BLEDataGenerator::BLEDataGenerator(std::in_place_t, T &&t)
    : BLEDataGenerator(
          std::in_place_type<std::remove_cv_t<std::remove_reference_t<T>>>,
          std::forward<T>(t)) {}

inline BLEDataGenerator::BLEDataGenerator(BLEDataGenerator &&other) noexcept {
    if (other.instance) {
        this->instance = other.instance->move_into(this->storage);
        other.clear();
    }
}

inline BLEDataGenerator &
BLEDataGenerator::operator=(BLEDataGenerator &&other) noexcept {
    clear();
    if (other.instance) {
        this->instance = other.instance->move_into(this->storage);
        other.clear();
    }
    return *this;
}

END_CS_NAMESPACE