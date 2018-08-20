#pragma once

#include <Helpers/LinkedList.hpp>
#include <stddef.h>
#include <Settings/SettingsWrapper.h>

class Updatable : public DoublyLinkable<Updatable> {
  public:
    Updatable() { updatables.append(this); }

    virtual ~Updatable() { updatables.remove(this); }

    virtual void update() = 0;

    virtual void begin() = 0;

    void enable() {
        if (isEnabled()) {
            DEBUGFN(F("Error: This element is already enabled."));
            ERROR(return );
        }
        updatables.append(this);
    }

    void disable() {
        if (!isEnabled()) {
            DEBUGFN(F("Error: This element is already disabled."));
            ERROR(return );
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

    template <class T, size_t N>
    static void enable(T (&array)[N]) {
        for (T &el : array)
            enable(el);
    }

    static void disable(Updatable *element) { element->disable(); }

    static void disable(Updatable &element) { element.disable(); }

    template <class T, size_t N>
    static void disable(T (&array)[N]) {
        for (T &el : array)
            disable(el);
    }

  private:
    static DoublyLinkedList<Updatable> updatables;
};