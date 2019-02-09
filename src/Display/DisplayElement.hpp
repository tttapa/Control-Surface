#pragma once

#include <Display/DisplayInterface.hpp>
#include <Helpers/LinkedList.hpp>

class DisplayElement : public DoublyLinkable<DisplayElement> {
  protected:
    DisplayElement(DisplayInterface &display) : display(display) {
        elements.insertSorted(
            this, [](const DisplayElement &lhs, const DisplayElement &rhs) {
                return &lhs.getDisplay() < &rhs.getDisplay();
            });
    }

  public:
    virtual ~DisplayElement() { elements.remove(this); }

    /** 
     * @brief   Draw this DisplayElement to the display buffer.
     */
    virtual void draw() = 0;

    DisplayInterface &getDisplay() { return display; }
    const DisplayInterface &getDisplay() const { return display; }

    static DoublyLinkedList<DisplayElement> &getAll() { return elements; }

  protected:
    DisplayInterface &display;

    static DoublyLinkedList<DisplayElement> elements;
};