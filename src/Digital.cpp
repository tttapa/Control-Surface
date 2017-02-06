#include "Digital.h"
#include "USBMidi.h"
#include "Arduino.h"

Digital::Digital(byte p, byte n, byte c, byte v){ // pin, controller number, channel, velocity
  pinMode(p, INPUT_PULLUP);
  pin = p; 
  note = n; 
  channel = c;
  velocity = v;
  oldVal = -1;
  bankTrue = false;
}    

void Digital::refresh(){
  value = digitalRead(pin) ^ invert;
  if(value != oldVal){
    if(value == 0){  // If the button is pressed
      if(bankTrue && !digitalRead(bankPin)){
        USBMidiController.send(NOTE_ON, newChannel, newNote, velocity);
      } 
      else  {
        USBMidiController.send(NOTE_ON, channel, note, velocity);
      }          
    } 
    else {  // If the button is not pressed
      if(bankTrue && !digitalRead(bankPin)){
        USBMidiController.send(NOTE_OFF, newChannel, newNote, velocity);
      } 
      else {
        USBMidiController.send(NOTE_OFF, channel, note, velocity);
      }
    }
    oldVal = value;
  }  
}

void Digital::bank(byte bPin, byte newN, byte newC) // bank pin, new controller or note number, new channel
{ 
  bankTrue = true;

  bankPin = bPin;
  pinMode(bankPin, INPUT_PULLUP);
  newNote = newN; 
  newChannel = newC;
}

void Digital::detachBank()
{
  bankTrue = false;
  pinMode(bankPin, INPUT); // make it a normal input again, without the internal pullup resistor.
}





