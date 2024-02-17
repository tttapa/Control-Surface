#pragma once

#include <AH/Containers/LinkedList.hpp>
#include <Display/DisplayInterface.hpp>

BEGIN_CS_NAMESPACE

/**
 * @brief   An interface for elements that draw to a display.
 * @ingroup DisplayElements
 */
class DisplayElement : public DoublyLinkable<DisplayElement> {
  protected:
    /// @brief  Create a new DisplayElement.
    ///
    /// @param  display
    ///         The display that this display element draws to.
    DisplayElement(DisplayInterface &display) : display(display) {
        // The elements are sorted by the address of their displays.
        // This way, all display elements that draw to the same display are next
        // to each other. This means that the display buffer can be reused, and
        // makes it easier to iterate over the displays and draw to them.
        elements.insertSorted(
            this, [](const DisplayElement &lhs, const DisplayElement &rhs) {
                return reinterpret_cast<uintptr_t>(&lhs.getDisplay()) <
                       reinterpret_cast<uintptr_t>(&rhs.getDisplay());
            });
    }

    /// @name Enabling and disabling display elements
    /// @{

    /// Enable this display element: insert it into the linked list of
    /// instances, so it gets drawn to the display
    void enable() {
        if (isEnabled()) {
            ERROR(F("Error: This element is already enabled."), 0x9212);
            return;
        }
        elements.append(this);
    }

    /// Disable this display element: remove it from the linked list of
    /// instances, so it no longer gets drawn to the display
    void disable() {
        if (!isEnabled()) {
            ERROR(F("Error: This element is already disabled."), 0x9213);
            return;
        }
        elements.remove(this);
    }

    /**
     * @brief   Check if this display element is enabled.
     * 
     * @note    Assumes that the element is not added to a different linked 
     *          list by the user.
     */
    bool isEnabled() const { return elements.couldContain(this); }

    /// @copydoc DisplayElement::enable
    static void enable(DisplayElement *element) { element->enable(); }
    /// @copydoc DisplayElement::enable
    static void enable(DisplayElement &element) { element.enable(); }
    /// @copydoc DisplayElement::enable
    template <class U, size_t N>
    static void enable(U (&array)[N]) {
        for (U &el : array)
            enable(el);
    }

    /// @copydoc DisplayElement::disable
    static void disable(DisplayElement *element) { element->disable(); }
    /// @copydoc DisplayElement::disable
    static void disable(DisplayElement &element) { element.disable(); }
    /// @copydoc DisplayElement::disable
    template <class U, size_t N>
    static void disable(U (&array)[N]) {
        for (U &el : array)
            disable(el);
    }

    /// @}

  public:
    virtual ~DisplayElement() { elements.remove(this); }

    /// Draw this DisplayElement to the display buffer.
    virtual void draw() = 0;

    /// Check if this DisplayElement has to be re-drawn.
    virtual bool getDirty() const = 0;

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