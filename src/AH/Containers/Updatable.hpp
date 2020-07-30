/* ✔ */

#pragma once

#include <AH/Settings/Warnings.hpp>

AH_DIAGNOSTIC_WERROR() // Enable errors on warnings

#include <AH/Containers/CRTP.hpp>
#include <AH/Containers/LinkedList.hpp>
#include <AH/Containers/Mutex.hpp>
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
template <class Derived, bool ThreadSafe = false>
class UpdatableCRTP : public DoublyLinkable<Derived> {

  public:
    using Mutex =
        typename std::conditional<ThreadSafe, DefaultMutEx, EmptyMutex>::type;
    using LockGuard =
        typename std::conditional<ThreadSafe, DefaultLockGuard<Mutex>,
                                  EmptyLockGuard<Mutex>>::type;

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
        LockGuard lock(mutex);
        updatables.append(CRTP(Derived));
    }

  public:
    /// Destructor: remove the updatable from the linked list of instances.
    virtual ~UpdatableCRTP() __attribute__((no_sanitize("undefined"))) {
        LockGuard lock(mutex);
        if (updatables.couldContain(CRTP(Derived)))
            updatables.remove(CRTP(Derived));
    }

#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic pop
#endif

  public:
    static Mutex &getMutex() { return mutex; }

    /// @name Main initialization and updating methods
    /// @{

    template <class... Args>
    static void applyToAll(const LockGuard &,
                           void (Derived::*method)(Args &&...),
                           Args &&... args) {
        for (auto &el : updatables)
            (el.*method)(std::forward<Args>(args)...);
    }

    template <class... Args>
    static void applyToAll(void (Derived::*method)(Args &&...),
                           Args &&... args) {
        applyToAll(LockGuard(mutex), method, std::forward<Args>(args)...);
    }

    /// @}

  public:
    /// @name Enabling and disabling updatables
    /// @{

    /// Enable this updatable: insert it into the linked list of instances,
    /// so it gets updated automatically
    void enable(const LockGuard &lock) {
        if (isEnabled(lock)) {
            ERROR(F("Error: This element is already enabled."), 0x1212);
            return;
        }
        updatables.append(CRTP(Derived));
    }

    /// @copydoc enable
    void enable() { enable(LockGuard(mutex)); }

    /// Disable this updatable: remove it from the linked list of instances,
    /// so it no longer gets updated automatically
    void disable(const LockGuard &lock) {
        if (!isEnabled(lock)) {
            ERROR(F("Error: This element is already disabled."), 0x1213);
            return;
        }
        updatables.remove(CRTP(Derived));
    }

    /// @copydoc disable
    void disable() { disable(LockGuard(mutex)); }

    /**
     * @brief   Check if this updatable is enabled.
     * 
     * @note    Assumes that the updatable is not added to a different linked 
     *          list by the user.
     */
    bool isEnabled(const LockGuard &) const {
        return updatables.couldContain(CRTP(const Derived));
    }

    /// @copydoc isEnabled
    bool isEnabled() { return isEnabled(LockGuard(mutex)); }

    /// @copydoc enable
    static void enable(UpdatableCRTP *element) { element->enable(); }
    /// @copydoc enable
    static void enable(UpdatableCRTP &element) { element.enable(); }
    /// @copydoc enable
    template <class U, size_t N>
    static void enable(U (&array)[N]) {
        for (U &el : array)
            enable(el);
    }

    /// @copydoc disable
    static void disable(UpdatableCRTP *element) { element->disable(); }
    /// @copydoc disable
    static void disable(UpdatableCRTP &element) { element.disable(); }
    /// @copydoc disable
    template <class U, size_t N>
    static void disable(U (&array)[N]) {
        for (U &el : array)
            disable(el);
    }

    /// Move down this element in the list.
    void moveDown(const LockGuard &) { updatables.moveDown(CRTP(Derived)); }
    /// Move down this element in the list.
    void moveDown() { moveDown(LockGuard(mutex)); }

    /// @}

  protected:
    static DoublyLinkedList<Derived> updatables;
    static Mutex mutex;
};

template <class Derived, bool ThreadSafe>
DoublyLinkedList<Derived> UpdatableCRTP<Derived, ThreadSafe>::updatables;

template <class Derived, bool ThreadSafe>
typename UpdatableCRTP<Derived, ThreadSafe>::Mutex
    UpdatableCRTP<Derived, ThreadSafe>::mutex;

struct NormalUpdatable {};

/**
 * @brief   A super class for object that have to be updated regularly.
 * 
 * All instances of this class are kept in a linked list, so it's easy to 
 * iterate over all of them to update them.
 * 
 * @nosubgrouping
 */
template <class T = NormalUpdatable, bool ThreadSafe = false>
class Updatable : public UpdatableCRTP<Updatable<T, ThreadSafe>, ThreadSafe> {
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

AH_DIAGNOSTIC_POP()
