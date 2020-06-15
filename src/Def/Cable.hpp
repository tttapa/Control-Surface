/* ✔ */

#pragma once

#include <stdint.h> // uint8_t
#include <Settings/NamespaceSettings.hpp>

BEGIN_CS_NAMESPACE

/** 
 * A type-safe class for MIDI USB Cable numbers.
 */
class Cable {
  public:
    /**
     * @brief   Create a MIDI Cable object.
     * 
     * @param   zeroBasedCable
     *          The zero-based cable (0 is the first cable).
     */
    explicit constexpr Cable(uint8_t zeroBasedCable)
        : zeroBasedCable(zeroBasedCable) {}

    /**
     * @brief   Get the cable as an integer.
     * 
     * @return  The zero-based cable (0 is the first cable).
     */
    constexpr uint8_t getRaw() const { return zeroBasedCable; }

    /**
     * @brief   Create a cable.
     * 
     * @param   oneBasedCable
     *          The cable number (1 is the first cable).
     */
    static constexpr Cable createCable(uint8_t oneBasedCable) {
        return Cable{uint8_t(oneBasedCable - 1)};
    }

    /**
     * @brief   Check if two cables are the same.
     * 
     * @param   rhs
     *          The other cable to compare this cable to.
     */
    constexpr bool operator==(const Cable &rhs) const {
        return this->zeroBasedCable == rhs.zeroBasedCable;
    }

    /**
     * @brief   Add an offset.
     * 
     * @param   rhs
     *          The offset to add to this cable.
     */
    Cable &operator+=(uint8_t rhs) {
        this->zeroBasedCable += rhs;
        return *this;
    }

    /**
     * @brief   Add an offset to a cable.
     * 
     * @param   rhs
     *          The offset to add to the cable.
     */
    Cable operator+(uint8_t rhs) const {
        Cable copy = *this;
        copy += rhs;
        return copy;
    }

    /**
     * @brief   Subtract an offset.
     * 
     * @param   rhs
     *          The offset to subtract from this cable.
     */
    Cable &operator-=(uint8_t rhs) {
        this->zeroBasedCable -= rhs;
        return *this;
    }

    /**
     * @brief   Subtract an offset from a cable.
     * 
     * @param   rhs
     *          The offset to subtract from the cable.
     */
    Cable operator-(uint8_t rhs) const {
        Cable copy = *this;
        copy -= rhs;
        return copy;
    }

  private:
    uint8_t zeroBasedCable : 4;
};

/** 
 * @brief   A literal operator to create MIDI Cables.
 * 
 * @param   cb
 *          The (one-based) MIDI cable (1 is the first cable).
 */
constexpr Cable operator"" _cb(unsigned long long cb) {
    return Cable::createCable(cb);
}

constexpr Cable CABLE_1 = 1_cb;
constexpr Cable CABLE_2 = 2_cb;
constexpr Cable CABLE_3 = 3_cb;
constexpr Cable CABLE_4 = 4_cb;
constexpr Cable CABLE_5 = 5_cb;
constexpr Cable CABLE_6 = 6_cb;
constexpr Cable CABLE_7 = 7_cb;
constexpr Cable CABLE_8 = 8_cb;
constexpr Cable CABLE_9 = 9_cb;
constexpr Cable CABLE_10 = 10_cb;
constexpr Cable CABLE_11 = 11_cb;
constexpr Cable CABLE_12 = 12_cb;
constexpr Cable CABLE_13 = 13_cb;
constexpr Cable CABLE_14 = 14_cb;
constexpr Cable CABLE_15 = 15_cb;
constexpr Cable CABLE_16 = 16_cb;

END_CS_NAMESPACE