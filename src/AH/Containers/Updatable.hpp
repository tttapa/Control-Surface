/* ✔ */

#pragma once

#include <AH/Settings/Warnings.hpp>

AH_DIAGNOSTIC_WERROR() // Enable errors on warnings

#include <AH/Containers/LinkedList.hpp>
#include <AH/Error/Error.hpp>
#include <AH/Settings/SettingsWrapper.hpp>
#include <stddef.h>

BEGIN_AH_NAMESPACE

struct NormalUpdatable {};

/**
 * @brief   A super class for object that have to be updated regularly.
 * 
 * All instances of this class are kept in a linked list, so it's easy to 
 * iterate over all of them to update them.
 */
template <class T = NormalUpdatable>
class Updatable : public DoublyLinkable<Updatable<T>> {
  protected:
    /// Create an Updatabe and add it to the linked list of instances.
    Updatable() { updatables.append(this); }

  public:
    /// Destructor: remove the updatable from the linked list of instances.
    virtual ~Updatable() {
        if (isEnabled())
            updatables.remove(this);
    }

    /// Update this updatable.
    virtual void update() = 0;

    /// Initialize this updatable.
    virtual void begin() = 0;

    /// Enable this updatable: insert it into the linked list of instances,
    /// so it gets updated automatically
    void enable() {
        if (isEnabled()) {
            ERROR(F("Error: This element is already enabled."), 0x1212);
            return;
        }
        updatables.append(this);
    }

    /// Disable this updatable: remove it from the linked list of instances,
    /// so it no longer gets updated automatically
    void disable() {
        if (!isEnabled()) {
            ERROR(F("Error: This element is already disabled."), 0x1213);
            return;
        }
        updatables.remove(this);
    }

    /**
     * @brief   Check if this updatable is enabled.
     * 
     * @note    Assumes that the updatable is not added to a different linked 
     *          list by the user.
     */
    bool isEnabled() { return updatables.couldContain(this); }

    /// Begin all enabled instances of this class
    /// @see    begin()
    static void beginAll() {
        for (Updatable &el : updatables)
            el.begin();
    }

    /// Update all enabled instances of this class
    /// @see    update()
    static void updateAll() {
        for (Updatable &el : updatables)
            el.update();
    }

    static void enable(Updatable *element) { element->enable(); }

    static void enable(Updatable &element) { element.enable(); }

    template <class U, size_t N>
    static void enable(U (&array)[N]) {
        for (U &el : array)
            enable(el);
    }

    static void disable(Updatable<T> *element) { element->disable(); }

    static void disable(Updatable<T> &element) { element.disable(); }

    template <class U, size_t N>
    static void disable(U (&array)[N]) {
        for (U &el : array)
            disable(el);
    }

  private:
    static DoublyLinkedList<Updatable<T>> updatables;
};

template <class T>
DoublyLinkedList<Updatable<T>> Updatable<T>::updatables;

END_AH_NAMESPACE

AH_DIAGNOSTIC_POP()
