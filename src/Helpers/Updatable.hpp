#pragma once

#include <Helpers/LinkedList.h>
#include <stddef.h>
#include <Settings/SettingsWrapper.h>

class Updatable : public Linkable<Updatable> {
  public:
    Updatable() { append(this, first, last); }

    virtual ~Updatable() { disable(); }

    virtual void update() = 0;

    virtual void begin() = 0;

    void enable() {
        if (isEnabled()) {
            DEBUGFN(F("Error: This element is already enabled."));
            ERROR(return );
        }
        append(this, first, last);
    }

    void disable() {
        if (!isEnabled()) {
            DEBUGFN(F("Error: This element is already disabled."));
            ERROR(return );
        }
        remove(this, first, last);
    }

    bool isEnabled() { return contains(this, first, last); }

    static void beginAll() {
        for (Updatable *el = first; el; el = el->next)
            el->begin();
    }

    static void updateAll() {
        for (Updatable *el = first; el; el = el->next)
            el->update();
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
    static Updatable *first;
    static Updatable *last;
};