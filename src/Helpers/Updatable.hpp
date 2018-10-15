#pragma once

#include <Helpers/Error.hpp>
#include <Helpers/LinkedList.hpp>
#include <Settings/SettingsWrapper.hpp>
#include <stddef.h>
#include <Def/Def.hpp>

template <class T = NormalUpdatable>
class Updatable : public DoublyLinkable<Updatable<T>> {
  public:
    Updatable() { updatables.append(this); }

    virtual ~Updatable() { updatables.remove(this); }

    virtual void update() = 0;

    virtual void begin() = 0;

    void enable() {
        if (isEnabled()) {
            ERROR(F("Error: This element is already enabled."), 0x1212);
            return;
        }
        updatables.append(this);
    }

    void disable() {
        if (!isEnabled()) {
            ERROR(F("Error: This element is already disabled."), 0x1213);
            return;
        }
        updatables.remove(this);
    }

    /**
     * @brief 
     * 
     * @note    Assumes that the updatable is not added to a different linked 
     *          list by the user.
     * 
     * @todo    Documentation
     * 
     * @return true 
     * @return false 
     */
    bool isEnabled() { return updatables.couldContain(this); }

    static void beginAll() {
        for (Updatable &el : updatables)
            el.begin();
    }

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