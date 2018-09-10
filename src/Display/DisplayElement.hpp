#pragma once

#include <Display/DisplayInterface.hpp>
#include <Helpers/LinkedList.hpp>

class DisplayElement : public DoublyLinkable<DisplayElement> {
  protected:
    DisplayElement(DisplayInterface &display) : display(display) {
        elements.append(this);
    }

  public:
    virtual ~DisplayElement() { elements.remove(this); }

    /** 
     * @brief   Draw this DisplayElement to the display buffer.
     */
    virtual void draw() = 0;

    /**
     * @brief   Draw all DisplayElement%s to their display buffers.
     * 
     * @see     DisplayElement#draw
     */
    static void drawAll() {
        for (DisplayElement &el : elements)
            el.draw();
    }

  protected:
    DisplayInterface &display;

    static DoublyLinkedList<DisplayElement> elements;
};