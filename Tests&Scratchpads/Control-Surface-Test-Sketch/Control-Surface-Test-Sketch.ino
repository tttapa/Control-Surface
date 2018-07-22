#include <Encoder.h>
#include <Control_Surface.h>

USBDebugMIDI_Interface udmi(115200);
// USBMIDI_Interface umi;

//-----------------------------------------------------------------------------------------------------

const pin_t dataPin  = 11; //Pin connected to DS of 74HC595
const pin_t clockPin = 13; //Pin connected to SH_CP of 74HC595
const pin_t latchPin = 10; //Pin connected to ST_CP of 74HC595

ShiftRegisterOut sr(dataPin, clockPin, latchPin, MSBFIRST, 8*3);
const uint8_t ShiftRegisterOut::redBit = 2;
const uint8_t ShiftRegisterOut::greenBit = 1;
const uint8_t ShiftRegisterOut::blueBit = 0;

//-----------------------------------------------------------------------------------------------------

Bankable::PBPotentiometer pbpot           = {A0,          1};
Bankable::CCPotentiometer ccpot           = {A1,   0x00,  1};

Bankable::NoteButton noteButton           = {5,    0x10,  1};
Bankable::NoteButtonLatching noteSwitch   = {6,    0x11,  1};
Bankable::CCButton ccButton               = {7,    0x12,  1};
Bankable::CCButtonLatching ccSwitch       = {8,    0x13,  1};

const int speedMultiply = 1;

const uint8_t NORMAL_ENCODER = 4; // TODO

Bankable::CCRotaryEncoder enc             = {{2, 3}, 0x20,  1, speedMultiply, NORMAL_ENCODER};

//-----------------------------------------------------------------------------------------------------

Bank bank(4); // A bank with four channels
BankSelector bankselector(bank, A2, 9, BankSelector::MOMENTARY);
  
//-----------------------------------------------------------------------------------------------------

// Bankable::Note_Input_LED<2> note = { 0x10, 1, sr.red(0) };

void setup() {
  bank.add(pbpot,      Bank::CHANGE_CHANNEL);
  bank.add(ccpot);
  bank.add(noteButton);
  bank.add(noteSwitch);
  bank.add(ccButton);
  bank.add(ccSwitch);
  bank.add(enc);
  // bank.add(note, Bank::CHANGE_ADDRESS);
  
  Control_Surface.begin();
  while(!Serial);
  
  DEBUG(F("Compiled at " __DATE__));
  DEBUG(F("From " __FILE__));
  DEBUG("");
  DEBUG(F("Size of void * =                      ") << sizeof(void *)                     << F(" bytes"));
  DEBUG(F("Size of int =                         ") << sizeof(int)                        << F(" bytes"));
  DEBUG(F("Size of long =                        ") << sizeof(long)                       << F(" bytes"));
  DEBUG(F("Size of double =                      ") << sizeof(double)                     << F(" bytes"));
  DEBUG("");
  DEBUG(F("Size of CCPotentiometer =             ") << sizeof(CCPotentiometer)            << F(" bytes"));
  DEBUG(F("Size of PBPotentiometer =             ") << sizeof(PBPotentiometer)            << F(" bytes"));
  DEBUG(F("Size of NoteButton =                  ") << sizeof(NoteButton)                 << F(" bytes"));
  DEBUG(F("Size of NoteButtonLatching =          ") << sizeof(NoteButtonLatching)         << F(" bytes"));
  DEBUG(F("Size of CCButton =                    ") << sizeof(CCButton)                   << F(" bytes"));
  DEBUG(F("Size of CCButtonLatching =            ") << sizeof(CCButtonLatching)           << F(" bytes"));
  DEBUG(F("Size of CCRotaryEncoder =             ") << sizeof(CCRotaryEncoder)            << F(" bytes"));
  DEBUG(F("Size of CCButtonMatrix<4,4> =         ") << sizeof(CCButtonMatrix<4,4>)        << F(" bytes"));
  DEBUG(F("Size of NoteButtonMatrix<4,4> =       ") << sizeof(NoteButtonMatrix<4,4>)      << F(" bytes"));
  DEBUG("");     
  DEBUG(F("Size of Bank =                        ") << sizeof(Bank)                       << F(" bytes"));
  DEBUG(F("Size of BankSelector =                ") << sizeof(BankSelector)               << F(" bytes"));
 // DEBUG("");
 // DEBUG(F("Size of Note_Input =                  ") << sizeof(Note_Input)                 << F(" bytes"));
 // DEBUG(F("Size of Bankable::Note_Input<2> =     ") << sizeof(Bankable::Note_Input<2>)    << F(" bytes"));
 // DEBUG(F("Size of Note_Input_LED =              ") << sizeof(Note_Input_LED)             << F(" bytes"));
 // DEBUG(F("Size of Bankable::Note_Input_LED<2> = ") << sizeof(Bankable::Note_Input_LED<2>)<< F(" bytes"));
  DEBUG("");
  DEBUG(F("--------------------------------------"                                           "--------") << endl);
}

//-----------------------------------------------------------------------------------------------------

void loop() {
  Control_Surface.loop();
}
