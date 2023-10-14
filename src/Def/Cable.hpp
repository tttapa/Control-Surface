/* ✔ */

#pragma once

#include <AH/Arduino-Wrapper.h> // Print
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
     * @brief   Get the cable as an integer.
     * 
     * @return  The one-based cable (1 is the first cable).
     */
    constexpr uint8_t getOneBased() const { return zeroBasedCable + 1; }

    /**
     * @brief   Create a cable.
     * 
     * @param   oneBasedCable
     *          The cable number (1 is the first cable).
     */
    static constexpr Cable createCable(uint8_t oneBasedCable) {
        return Cable {static_cast<uint8_t>(oneBasedCable - 1)};
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
     * @brief   Check if two cables are the different.
     * 
     * @param   rhs
     *          The other cable to compare this cable to.
     */
    constexpr bool operator!=(const Cable &rhs) const {
        return this->zeroBasedCable != rhs.zeroBasedCable;
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

constexpr Cable Cable_1 = Cable::createCable(1);
constexpr Cable Cable_2 = Cable::createCable(2);
constexpr Cable Cable_3 = Cable::createCable(3);
constexpr Cable Cable_4 = Cable::createCable(4);
constexpr Cable Cable_5 = Cable::createCable(5);
constexpr Cable Cable_6 = Cable::createCable(6);
constexpr Cable Cable_7 = Cable::createCable(7);
constexpr Cable Cable_8 = Cable::createCable(8);
constexpr Cable Cable_9 = Cable::createCable(9);
constexpr Cable Cable_10 = Cable::createCable(10);
constexpr Cable Cable_11 = Cable::createCable(11);
constexpr Cable Cable_12 = Cable::createCable(12);
constexpr Cable Cable_13 = Cable::createCable(13);
constexpr Cable Cable_14 = Cable::createCable(14);
constexpr Cable Cable_15 = Cable::createCable(15);
constexpr Cable Cable_16 = Cable::createCable(16);

constexpr Cable CABLE_1 CS_DEPREC("Use Cable_1 instead") = Cable_1;
constexpr Cable CABLE_2 CS_DEPREC("Use Cable_2 instead") = Cable_2;
constexpr Cable CABLE_3 CS_DEPREC("Use Cable_3 instead") = Cable_3;
constexpr Cable CABLE_4 CS_DEPREC("Use Cable_4 instead") = Cable_4;
constexpr Cable CABLE_5 CS_DEPREC("Use Cable_5 instead") = Cable_5;
constexpr Cable CABLE_6 CS_DEPREC("Use Cable_6 instead") = Cable_6;
constexpr Cable CABLE_7 CS_DEPREC("Use Cable_7 instead") = Cable_7;
constexpr Cable CABLE_8 CS_DEPREC("Use Cable_8 instead") = Cable_8;
constexpr Cable CABLE_9 CS_DEPREC("Use Cable_9 instead") = Cable_9;
constexpr Cable CABLE_10 CS_DEPREC("Use Cable_10 instead") = Cable_10;
constexpr Cable CABLE_11 CS_DEPREC("Use Cable_11 instead") = Cable_11;
constexpr Cable CABLE_12 CS_DEPREC("Use Cable_12 instead") = Cable_12;
constexpr Cable CABLE_13 CS_DEPREC("Use Cable_13 instead") = Cable_13;
constexpr Cable CABLE_14 CS_DEPREC("Use Cable_14 instead") = Cable_14;
constexpr Cable CABLE_15 CS_DEPREC("Use Cable_15 instead") = Cable_15;
constexpr Cable CABLE_16 CS_DEPREC("Use Cable_16 instead") = Cable_16;

Print &operator<<(Print &, Cable);

END_CS_NAMESPACE