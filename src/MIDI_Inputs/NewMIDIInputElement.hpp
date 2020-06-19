#pragma once

#include "ChannelMessageMatcher.hpp"
#include <Def/MIDIAddress.hpp>

#include <Banks/Bank.hpp> // Bank<N>, BankSettingChangeCallback

#include <AH/Containers/Updatable.hpp>
#include <AH/STL/type_traits>

BEGIN_CS_NAMESPACE

// -------------------------------------------------------------------------- //

/**
 * @brief   A class for objects that listen for incoming MIDI events.
 * 
 * They can either update some kind of display, or they can just save the state.
 */
template <MIDIMessageType Type>
class NewMIDIInputElement
    : public AH::UpdatableCRTP<NewMIDIInputElement<Type>, true> {
  protected:
    NewMIDIInputElement() = default;

  public:
    virtual ~NewMIDIInputElement() = default;

  public:
    using MessageMatcherType =
        typename std::conditional<Type == MIDIMessageType::SYSEX_START,
                                  SysExMessage, ChannelMessageMatcher>::type;

    /// Initialize the input element.
    virtual void begin() {}

    /// Reset the input element to its initial state.
    virtual void reset() {}

    /// Update the value of the input element. Used for decaying VU meters etc.
    virtual void update() {}

    /// Receive a new MIDI message and update the internal state.
    virtual bool updateWith(MessageMatcherType midimsg) = 0;

    /// Update all
    static bool updateAllWith(MessageMatcherType midimsg) {
        // MIDI messages can arrive asynchronously, so the linked list must be
        // locked before iterating over it or altering it.
        typename NewMIDIInputElement::LockGuard lock(
            NewMIDIInputElement::getMutex());

        for (auto &el : NewMIDIInputElement::updatables) {
            if (el.updateWith(midimsg)) {
                el.moveDown(lock);
                return true;
            }
        }
        return false;
    }

    /// Update all
    static void updateAll() {
        NewMIDIInputElement::applyToAll(&NewMIDIInputElement::update);
    }

    /// Begin all
    static void beginAll() {
        NewMIDIInputElement::applyToAll(&NewMIDIInputElement::begin);
    }
};

// -------------------------------------------------------------------------- //

template <MIDIMessageType Type, class Matcher>
class MatchingMIDIInputElement : public NewMIDIInputElement<Type> {
  protected:
    MatchingMIDIInputElement(const Matcher &matcher) : matcher(matcher) {}

  public:
    using MessageMatcherType =
        typename NewMIDIInputElement<Type>::MessageMatcherType;

    bool updateWith(MessageMatcherType midimsg) override {
        auto match = matcher(midimsg);
        if (match.match)
            handleUpdate(match);
        return match.match;
    }

    virtual void handleUpdate(typename Matcher::Result match) = 0;

  protected:
    Matcher matcher;
};

// -------------------------------------------------------------------------- //

template <MIDIMessageType Type, class Matcher>
class BankableMatchingMIDIInputElement
    : public MatchingMIDIInputElement<Type, Matcher>,
      public BankSettingChangeCallback {
    friend class Bank<Matcher::getBankSize()>;

  protected:
    /// Create a new BankableMatchingMIDIInputElement object, and add it to the
    /// bank.
    BankableMatchingMIDIInputElement(const Matcher &matcher)
        : MatchingMIDIInputElement<Type, Matcher>(matcher) {
        this->matcher.getBank().add(this);
    }

  public:
    /// Destructor: remove element from the bank.
    virtual ~BankableMatchingMIDIInputElement() {
        this->matcher.getBank().remove(this);
    }
};

// -------------------------------------------------------------------------- //

END_CS_NAMESPACE