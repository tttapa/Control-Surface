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
class MIDIInputElement : public AH::UpdatableCRTP<MIDIInputElement<Type>> {
  protected:
    MIDIInputElement() = default;

  public:
    virtual ~MIDIInputElement() = default;

  public:
    using MessageType =
        typename std::conditional<Type == MIDIMessageType::SysExStart,
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
        for (auto &el : MIDIInputElement::updatables) {
            if (el.updateWith(midimsg)) {
                el.moveDown();
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

/// The @ref MIDIInputElement base class is very general: you give it a MIDI
/// message, and it calls the `updateWith()` method with that message. Each
/// instance must then determine whether the message is meant for them or not.
/// This is often a very repetitive task, so that logic is best isolated in a
/// so-called “Matcher”. The Matcher looks at the MIDI message, checks if it
/// matches its MIDI address, for example, and if so, it extracts some data
/// (such as the MIDI velocity value from the message). Then it returns whether
/// it matched and the extra data as a “Matcher::Result” object. If the message
/// matched, that Result object is passed to the
/// @ref MatchingMIDIInputElement::handleUpdate() method, so it can be handled
/// there.
///
/// @todo   Pass the MIDI message to the @ref handleUpdate() method.
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

/// Similar to @ref MatchingMIDIInputElement, but for Bankable MIDI Input
/// Elements.
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

/// MIDI Input Element that listens for MIDI Note On/Off messages.
using MIDIInputElementNote = MIDIInputElement<MIDIMessageType::NoteOn>;
/// MIDI Input Element that listens for MIDI Key Pressure messages.
using MIDIInputElementKP = MIDIInputElement<MIDIMessageType::KeyPressure>;
/// MIDI Input Element that listens for MIDI Control Change messages.
using MIDIInputElementCC = MIDIInputElement<MIDIMessageType::ControlChange>;
/// MIDI Input Element that listens for MIDI Program Change messages.
using MIDIInputElementPC = MIDIInputElement<MIDIMessageType::ProgramChange>;
/// MIDI Input Element that listens for MIDI Channel Pressure messages.
using MIDIInputElementCP = MIDIInputElement<MIDIMessageType::ChannelPressure>;
/// MIDI Input Element that listens for MIDI Pitch Bend messages.
using MIDIInputElementPB = MIDIInputElement<MIDIMessageType::PitchBend>;
/// MIDI Input Element that listens for MIDI System Exclusive messages.
using MIDIInputElementSysEx = MIDIInputElement<MIDIMessageType::SysExStart>;

END_CS_NAMESPACE
