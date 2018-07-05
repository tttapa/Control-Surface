#pragma once

#include "../Helpers/LinkedList.h"
#include "../MIDI_Element.h"
#include "./MIDI_message_matcher.h"

// -------------------------------------------------------------------------- //

class MIDI_Input_Element : public MIDI_Element {
    friend class Control_Surface_;

  public:
    MIDI_Input_Element(uint8_t address, uint8_t channel,
                       uint8_t nb_addresses = 1, uint8_t nb_channels = 1);

    void setChannelOffset(uint8_t offset) override;
    void setAddressOffset(uint8_t offset) override;

  protected:
    /**
     * @brief   Update this MIDI_Input_Element with an incoming MIDI message,
     *          if the given message matches this element's channel and address.
     *
     * If the channel and address of the message matches this element,
     * MIDI_Input_Element::updateImpl is executed, and the internal state is
     * updated with the new data from the MIDI message.
     *
     * The element moves down in the linked list of MIDI_Input_Elements,
     * so objects that match regularly, will be found faster. (Matching an
     * incoming MIDI message with the elements is basically linear search.)
     *
     * @param   midimsg
     *          The new incoming MIDI message to match against this element.
     * @return  true
     *          The MIDI message matches this element and the internal state
     *          is updated.
     * @return  false
     *          The MIDI message does not match this element. Keep on looking
     *          for an element that does match it.
     */
    virtual bool updateWith(const MIDI_message_matcher &midimsg);

    /**
     * @brief   Write the internal of the element to its output.
     *
     * @note    This function is not called on each update of the
     *          Control_Surface. It is only called when the internal state
     *          changes (i.e. in MIDI_Input_Element::updateImpl or in
     *          MIDI_Input_Element::update).
     */
    virtual void display() = 0;

    /**
     * @brief   Check if the given incoming MIDI message matches the channel
     *          and address of this element.
     *
     *
     * @param   midimsg
     *          The new incoming MIDI message to match against this element.
     * @return  true
     *          The channel and address of the MIDI message match one of the
     *          channel/address combinations that this element is listening on.
     * @return  false
     *          The channel and address of the MIDI message doesn't match any
     *          channel/address combination that this element is listening on.
     */
    virtual bool match(const MIDI_message_matcher &midimsg);

    /**
     * @brief   This function is executed when an incoming MIDI message matches
     *          this element, and it should update the internal state of this
     *          element with the new data of the MIDI message.
     *
     * @param   midimsg
     *          The new incoming MIDI message to update the internal state of
     *          the element with.
     * @return  true
     *          The MIDI message was indeed meant for this element, and the
     *          state has been updated.
     * @return  false
     *          Even though the channel and address of the MIDI match this
     *          element, the additional addressing information in the data
     *          bytes of the MIDI message does not match this element, and
     *          the internal state is not updated.
     */
    virtual bool updateImpl(const MIDI_message_matcher &midimsg) = 0;

    /**
     * @brief   Check if the given target address matches one of the addresses
     *          that this element is listening on.
     *
     * @param   targetAddress
     *          The address to check against this element's addresses.
     * @return  true
     *          This element listens on the given target address.
     * @return  false
     *          This element doesn't listen on the given target address.
     */
    bool matchAddress(uint8_t targetAddress);
    
    /**
     * @brief   Check if the given target channel matches one of the channels
     *          that this element is listening on.
     *
     * @param   targetChannel
     *          The channel to check against this element's channels.
     * @return  true
     *          This element listens on the given target channel.
     * @return  false
     *          This element doesn't listen on the given target channel.
     */
    bool matchChannel(uint8_t targetChannel);

    const uint8_t address;
    const uint8_t channel;
    const uint8_t nb_addresses;
    const uint8_t nb_channels;

    /**
     * @brief   Move down this element in the linked list of elements.
     *          This means that the element will be checked earlier on the next
     *          iteration. 
     */
    virtual void moveDown() {}
};

// -------------------------------------------------------------------------- //

class MIDI_Input_Element_CC : public MIDI_Input_Element {
    friend class Control_Surface_;

  public:
    MIDI_Input_Element_CC(uint8_t address, uint8_t channel,
                          uint8_t nb_addresses, uint8_t nb_channels);
    ~MIDI_Input_Element_CC();

    static MIDI_Input_Element_CC *getFirst();
    static MIDI_Input_Element_CC *getLast();
    MIDI_Input_Element_CC *getNext();

  protected:
    virtual void update() override = 0;

    MIDI_Input_Element_CC *next = nullptr, *previous = nullptr;
    static MIDI_Input_Element_CC *last;
    static MIDI_Input_Element_CC *first;

    void moveDown();

    template <class Node>
    friend void LinkedList::append(Node *, Node *&, Node *&);
    template <class Node>
    friend void LinkedList::moveDown(Node *, Node *&, Node *&);
    template <class Node>
    friend void LinkedList::remove(Node *, Node *&, Node *&);
};

// -------------------------------------------------------------------------- //

class MIDI_Input_Element_Note : public MIDI_Input_Element {
    friend class Control_Surface_;

  public:
    MIDI_Input_Element_Note(uint8_t address, uint8_t channel,
                            uint8_t nb_addresses, uint8_t nb_channels);
    ~MIDI_Input_Element_Note();

    static MIDI_Input_Element_Note *getFirst();
    static MIDI_Input_Element_Note *getLast();
    MIDI_Input_Element_Note *getNext();

  protected:
    virtual void update() override = 0;

    MIDI_Input_Element_Note *next = nullptr, *previous = nullptr;
    static MIDI_Input_Element_Note *last;
    static MIDI_Input_Element_Note *first;

    void moveDown();

    template <class Node>
    friend void LinkedList::append(Node *, Node *&, Node *&);
    template <class Node>
    friend void LinkedList::moveDown(Node *, Node *&, Node *&);
    template <class Node>
    friend void LinkedList::remove(Node *, Node *&, Node *&);
};

// -------------------------------------------------------------------------- //

class MIDI_Input_Element_ChannelPressure : public MIDI_Input_Element {
    friend class Control_Surface_;

  public:
    MIDI_Input_Element_ChannelPressure(uint8_t address, uint8_t channel,
                                       uint8_t nb_addresses,
                                       uint8_t nb_channels);
    ~MIDI_Input_Element_ChannelPressure();

    static MIDI_Input_Element_ChannelPressure *getFirst();
    static MIDI_Input_Element_ChannelPressure *getLast();
    MIDI_Input_Element_ChannelPressure *getNext();

    bool match(const MIDI_message_matcher &midimsg);

  protected:
    MIDI_Input_Element_ChannelPressure *next = nullptr, *previous = nullptr;
    static MIDI_Input_Element_ChannelPressure *last;
    static MIDI_Input_Element_ChannelPressure *first;

    void moveDown();

    template <class Node>
    friend void LinkedList::append(Node *, Node *&, Node *&);
    template <class Node>
    friend void LinkedList::moveDown(Node *, Node *&, Node *&);
    template <class Node>
    friend void LinkedList::remove(Node *, Node *&, Node *&);
};