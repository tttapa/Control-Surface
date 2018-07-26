#pragma once

#include "Selectable.hpp"
#include <Helpers/Debug.hpp>
#include <Helpers/LinkedList.h>

class Selector {
  protected:
    Selector(Selectable &selectable, setting_t numberOfSettings)
        : selectable(selectable), numberOfSettings(numberOfSettings) {
        LinkedList::append(this, first, last);
    }

  public:
    virtual ~Selector() { LinkedList::remove(this, first, last); }

    virtual void begin() = 0;
    static void beginAll() {
        for (Selector *sel = first; sel; sel = sel->next)
            sel->begin();
    }

    virtual void update() = 0;
    static void updateAll() {
        for (Selector *sel = first; sel; sel = sel->next)
            sel->update();
    }

    virtual void reset() { set(0); }

    setting_t get() const { return setting; }

    void set(setting_t newSetting) {
        newSetting = validateSetting(newSetting);
        onSettingChange(get(), newSetting);
        updateDisplay(get(), newSetting);
        setting = newSetting;
    }

    setting_t getNumberOfSettings() const { return numberOfSettings; }

  private:
    virtual void onSettingChange(setting_t oldSetting, setting_t newSetting) {
        selectable.select(newSetting);
    }
    virtual void updateDisplay(setting_t oldSetting, setting_t newSetting) {}

    setting_t validateSetting(setting_t setting) const {
        if (setting >= numberOfSettings) {
            DEBUGFN(F("Error: Setting ")
                    << setting
                    << F(" is not less than the total number of settings (")
                    << numberOfSettings << ')');
            ERROR(return numberOfSettings - 1);
        }
        return setting;
    }

    Selectable &selectable;
    const setting_t numberOfSettings;
    setting_t setting = 0;

    Selector *next = nullptr;
    Selector *previous = nullptr;
    static Selector *last;
    static Selector *first;

    template <class Node>
    friend void LinkedList::append(Node *, Node *&, Node *&);
    template <class Node>
    friend void LinkedList::remove(Node *, Node *&, Node *&);
};