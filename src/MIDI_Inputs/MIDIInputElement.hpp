#pragma once

#include <Def/MIDIAddress.hpp>
#include <MIDI_Parsers/MIDI_MessageTypes.hpp>

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
class MIDIInputElement
    : public AH::UpdatableCRTP<MIDIInputElement<Type>, true> {
  protected:
    MIDIInputElement() = default;

  public:
    virtual ~MIDIInputElement() = default;

  public:
    using MessageType =
        typename std::conditional<Type == MIDIMessageType::SYSEX_START,
                                  SysExMessage, ChannelMessage>::type;

    /// Initialize the input element.
    virtual void begin() {} // LCOV_EXCL_LINE

    /// Reset the input element to its initial state.
    virtual void reset() {} // LCOV_EXCL_LINE

    /// Update the value of the input element. Used for decaying VU meters etc.
    virtual void update() {} // LCOV_EXCL_LINE

    /// Receive a new MIDI message and update the internal state.
    virtual bool updateWith(MessageType midimsg) = 0;

    /// Update all
    static bool updateAllWith(MessageType midimsg) {
        // MIDI messages can arrive asynchronously, so the linked list must be
        // locked before iterating over it or altering it.
        typename MIDIInputElement::LockGuard lock(MIDIInputElement::getMutex());

        for (auto &el : MIDIInputElement::updatables) {
            if (el.updateWith(midimsg)) {
                el.moveDown(lock);
                return true;
            }
        }
        return false;
    }

    /// Update all
    static void updateAll() {
        MIDIInputElement::applyToAll(&MIDIInputElement::update);
    }

    /// Begin all
    static void beginAll() {
        MIDIInputElement::applyToAll(&MIDIInputElement::begin);
    }

    /// Reset all
    static void resetAll() {
        MIDIInputElement::applyToAll(&MIDIInputElement::reset);
    }
};

// -------------------------------------------------------------------------- //

template <MIDIMessageType Type, class Matcher>
class MatchingMIDIInputElement : public MIDIInputElement<Type> {
  protected:
    MatchingMIDIInputElement(const Matcher &matcher) : matcher(matcher) {}

  public:
    using MessageType = typename MIDIInputElement<Type>::MessageType;

    bool updateWith(MessageType midimsg) override {
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

    uint8_t getActiveBank() const { return this->matcher.getSelection(); }

  public:
    /// Destructor: remove element from the bank.
    virtual ~BankableMatchingMIDIInputElement() {
        this->matcher.getBank().remove(this);
    }
};

// -------------------------------------------------------------------------- //

using MIDIInputElementNote = MIDIInputElement<MIDIMessageType::NOTE_ON>;
using MIDIInputElementKP = MIDIInputElement<MIDIMessageType::KEY_PRESSURE>;
using MIDIInputElementCC = MIDIInputElement<MIDIMessageType::CONTROL_CHANGE>;
using MIDIInputElementPC = MIDIInputElement<MIDIMessageType::PROGRAM_CHANGE>;
using MIDIInputElementCP = MIDIInputElement<MIDIMessageType::CHANNEL_PRESSURE>;
using MIDIInputElementPB = MIDIInputElement<MIDIMessageType::PITCH_BEND>;
using MIDIInputElementSysEx = MIDIInputElement<MIDIMessageType::SYSEX_START>;

END_CS_NAMESPACE