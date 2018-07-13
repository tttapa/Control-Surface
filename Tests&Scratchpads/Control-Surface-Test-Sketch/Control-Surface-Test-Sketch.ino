#include <Encoder.h>
#include <Control_Surface.h>

USBDebugMIDI_Interface udmi(115200);
// USBMIDI_Interface umi;

const int speedMultiply = 1;

//-----------------------------------------------------------------------------------------------------

PBPotentiometer pbpot           = {A0,          1};
CCPotentiometer ccpot           = {A1,   0x00,  1};

NoteButton noteButton           = {5,    0x10,  1};
NoteButtonLatching noteSwitch   = {6,    0x11,  1};
CCButton ccButton               = {7,    0x12,  1};
CCButtonLatching ccSwitch       = {8,    0x13,  1};

RotaryEncoder enc               = {2, 3, 0x20,  1, speedMultiply, NORMAL_ENCODER, TWOS_COMPLEMENT};

//-----------------------------------------------------------------------------------------------------

Bank bank(4); // A bank with four channels
BankSelector bankselector(bank, A2, LED_BUILTIN, BankSelector::MOMENTARY);

//-----------------------------------------------------------------------------------------------------

Note_Input_Bankable_LED<2> note = { 0x10, 1, A3 };

void setup() {
  // // bank.add(pbpot,      Bank::CHANGE_CHANNEL);
  // // bank.add(noteButton, Bank::CHANGE_ADDRESS);
  bank.add(note, Bank::CHANGE_ADDRESS);
  
  Control_Surface.begin();
  while(!Serial);
  DEBUG(F("Size of CCPotentiometer =       ") << sizeof(CCPotentiometer)       << F(" bytes"));
  DEBUG(F("Size of PBPotentiometer =       ") << sizeof(PBPotentiometer)       << F(" bytes"));
  DEBUG(F("Size of NoteButton =            ") << sizeof(NoteButton)            << F(" bytes"));
  DEBUG(F("Size of NoteButtonLatching =    ") << sizeof(NoteButtonLatching)    << F(" bytes"));
  DEBUG(F("Size of CCButton =              ") << sizeof(CCButton)              << F(" bytes"));
  DEBUG(F("Size of CCButtonLatching =      ") << sizeof(CCButtonLatching)      << F(" bytes"));
  DEBUG(F("Size of RotaryEncoder =         ") << sizeof(RotaryEncoder)         << F(" bytes"));
  DEBUG(F("Size of CCButtonMatrix<4,4> =   ") << sizeof(CCButtonMatrix<4,4>)   << F(" bytes"));
  DEBUG(F("Size of NoteButtonMatrix<4,4> = ") << sizeof(NoteButtonMatrix<4,4>) << F(" bytes"));
  DEBUG("");
  DEBUG(F("Size of Bank =                  ") << sizeof(Bank)                  << F(" bytes"));
  DEBUG(F("Size of BankSelector =          ") << sizeof(BankSelector)          << F(" bytes"));
  DEBUG(F("--------------------------------"                                      "--------") << endl);
}

//-----------------------------------------------------------------------------------------------------

void loop() {
  Control_Surface.loop();
}
