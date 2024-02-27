/* ✔ */

#pragma once

#include <AH/Containers/CRTP.hpp>
#include <AH/Containers/LinkedList.hpp>
#include <AH/Error/Error.hpp>
#include <AH/STL/type_traits>
#include <AH/STL/utility> // std::forward
#include <AH/Settings/SettingsWrapper.hpp>
#include <stddef.h>

BEGIN_AH_NAMESPACE

/**
 * @brief   A super class for object that have to be updated regularly.
 * 
 * All instances of this class are kept in a linked list, so it's easy to 
 * iterate over all of them to update them.  
 * 
 * This version uses static polymorphism using the Curiously Recurring Template
 * Pattern. This requires less virtual function calls.  
 * (Only the destructor is virtual.)
 * 
 * @nosubgrouping
 */
template <class Derived>
class UpdatableCRTP : public DoublyLinkable<Derived> {

  public:
#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
#endif

    // When a Derived instance is constructed, the base class constructor
    // UpdatableCRTP is called first. Because the Derived constructor hasn't
    // been run yet, the dynamic type is just Updatable, not yet Derived.
    // The undefined behavior sanitizer checks this dynamic type when the this
    // pointer is casted to Derived using the CRTP macro, and thus causes an
    // error.
    // The constructor only casts and stores the pointer, it doesn't dereference
    // it to call any of Derived methods, so I don't think that it's undefined
    // behavior (and I don't know of a better way to do this).
    // Also see https://stackoverflow.com/q/61061051/6356744

  protected:
    /// Constructor: create an Updatable and add it to the linked list of
    /// instances.
    UpdatableCRTP() __attribute__((no_sanitize("undefined"))) {
        updatables.append(CRTP(Derived));
    }

    UpdatableCRTP(const UpdatableCRTP &)
        __attribute__((no_sanitize("undefined")))
        : DoublyLinkable<Derived>() {
        updatables.append(CRTP(Derived));
    }
    UpdatableCRTP &operator=(const UpdatableCRTP &) { return *this; }

    UpdatableCRTP(UpdatableCRTP &&) __attribute__((no_sanitize("undefined"))) {
        updatables.append(CRTP(Derived));
    }
    UpdatableCRTP &operator=(UpdatableCRTP &&) { return *this; }

  public:
    /// Destructor: remove the updatable from the linked list of instances.
    virtual ~UpdatableCRTP() __attribute__((no_sanitize("undefined"))) {
        if (updatables.couldContain(CRTP(Derived)))
            updatables.remove(CRTP(Derived));
    }

#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic pop
#endif

  public:
    /// @name Main initialization and updating methods
    /// @{

    template <class... Args>
    static void __attribute__((always_inline))
    applyToAll(void (Derived::*method)(Args...), Args... args) {
        for (auto &el : updatables)
            (el.*method)(args...);
    }

    /// @}

  public:
    /// @name Enabling and disabling updatables
    /// @{

    /// Enable this updatable: insert it into the linked list of instances,
    /// so it gets updated automatically
    void enable() {
        if (isEnabled()) {
            ERROR(F("Error: This element is already enabled."), 0x1212);
            return; // LCOV_EXCL_LINE
        }
        updatables.append(CRTP(Derived));
    }

    /// Disable this updatable: remove it from the linked list of instances,
    /// so it no longer gets updated automatically
    void disable() {
        if (!isEnabled()) {
            ERROR(F("Error: This element is already disabled."), 0x1213);
            return; // LCOV_EXCL_LINE
        }
        updatables.remove(CRTP(Derived));
    }

    /**
     * @brief   Check if this updatable is enabled.
     * 
     * @note    Assumes that the updatable is not added to a different linked 
     *          list by the user.
     */
    bool isEnabled() const {
        return updatables.couldContain(CRTP(const Derived));
    }

    /// @copydoc enable()
    static void enable(UpdatableCRTP *element) { element->enable(); }
    /// @copydoc enable()
    static void enable(UpdatableCRTP &element) { element.enable(); }
    /// @copydoc enable()
    template <class U, size_t N>
    static void enable(U (&array)[N]) {
        for (U &el : array)
            enable(el);
    }

    /// @copydoc disable()
    static void disable(UpdatableCRTP *element) { element->disable(); }
    /// @copydoc disable()
    static void disable(UpdatableCRTP &element) { element.disable(); }
    /// @copydoc disable()
    template <class U, size_t N>
    static void disable(U (&array)[N]) {
        for (U &el : array)
            disable(el);
    }

    /// Move down this element in the list.
    void moveDown() { updatables.moveDown(CRTP(Derived)); }

    /// @}

  protected:
    static DoublyLinkedList<Derived> updatables;
};

template <class Derived>
DoublyLinkedList<Derived> UpdatableCRTP<Derived>::updatables;

struct NormalUpdatable {};

/**
 * @brief   A super class for object that have to be updated regularly.
 * 
 * All instances of this class are kept in a linked list, so it's easy to 
 * iterate over all of them to update them.
 * 
 * @nosubgrouping
 */
template <class T = NormalUpdatable>
class Updatable : public UpdatableCRTP<Updatable<T>> {
  public:
    /// @name Main initialization and updating methods
    /// @{

    /// Initialize this updatable.
    virtual void begin() = 0;

    /// Update this updatable.
    virtual void update() = 0;

    /// Begin all enabled instances of this class
    /// @see    begin()
    static void beginAll() { Updatable::applyToAll(&Updatable::begin); }

    /// Update all enabled instances of this class
    /// @see    update()
    static void updateAll() { Updatable::applyToAll(&Updatable::update); }

    /// @}
};

END_AH_NAMESPACE
