#pragma once

#include <Display/DisplayInterface.hpp>
#include <AH/Containers/LinkedList.hpp>

BEGIN_CS_NAMESPACE

/**
 * @brief   An interface for elements that draw to a display.
 */
class DisplayElement : public DoublyLinkable<DisplayElement> {
  protected:
    /**
     * @brief   Create a new DisplayElement.
     * 
     * @param   display
     *          The display that this display element draws to.
     */
    DisplayElement(DisplayInterface &display) : display(display) {
        // The elements are sorted by the address of their displays.
        // This way, all display elements that draw to the same display are next
        // to each other. This means that the display buffer can be reused.
        elements.insertSorted(
            this, [](const DisplayElement &lhs, const DisplayElement &rhs) {
                return &lhs.getDisplay() < &rhs.getDisplay();
            });
    }

  public:
    virtual ~DisplayElement() { elements.remove(this); }

    /// Draw this DisplayElement to the display buffer.
    virtual void draw() = 0;

    /// Get a reference to the display that this element draws to.
    DisplayInterface &getDisplay() { return display; }
    /// Get a const reference to the display that this element draws to.
    const DisplayInterface &getDisplay() const { return display; }

    /// Get the list of all DisplayElement instances.
    static DoublyLinkedList<DisplayElement> &getAll() { return elements; }

  protected:
    DisplayInterface &display;

    static DoublyLinkedList<DisplayElement> elements;
};

END_CS_NAMESPACE