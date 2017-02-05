#include "Arduino.h"
#include "DigitalLatch.h"
#include "USBMidi.h"

DigitalLatch::DigitalLatch(byte p, byte n, byte c, byte v, int l){ // pin, controller number, channel, latch time (ms)
  pinMode(p, INPUT_PULLUP);
  pin = p; 
  note = n; 
  channel = c;
  velocity = v;
  oldVal = 1;
  latch = l;
  offSent = true;
}  

void DigitalLatch::refresh(){
  value = digitalRead(pin);
  if(value != oldVal){
    if(offSent){  // If the note is turned off
      if(bankTrue && !digitalRead(bankPin)){
        USB.send(NOTE_ON, newChannel, newNote, velocity);
      } 
      else 
      {
        USB.send(NOTE_ON, channel, note, velocity);
      }
      time = millis();  // store the time of the note on message
      offSent = false; // The note is turned on
    } 
    else { // If the button is switched again, before latch time is reached
      if(bankTrue && !digitalRead(bankPin)){
        USB.send(NOTE_OFF, newChannel, newNote, velocity);  // Turn off the note 
      } 
      else 
      {
        USB.send(NOTE_OFF, channel, note, velocity);  // Turn off the note 
      }
      if(bankTrue && !digitalRead(bankPin)){
        USB.send(NOTE_ON, newChannel, newNote, velocity);   // Turn the note on again
      } 
      else 
      {
        USB.send(NOTE_ON, channel, note, velocity);   // Turn the note on again
      }
      time = millis();  // store the time of the note on message
    }
    oldVal = value;
  }
  if(millis() - time > latch && !offSent){  // if the elapsed time since the note on message is greater than the latch time, and if the note is still not turned off
    if(bankTrue && !digitalRead(bankPin)){
      USB.send(NOTE_OFF, newChannel, newNote, velocity);  // Turn off the note 
    } 
    else 
    {
      USB.send(NOTE_OFF, channel, note, velocity);  // Turn off the note 
    }      
    offSent = true; // The note is turned off
  }
}

void DigitalLatch::bank(byte bPin, byte newN, byte newC)  // bank pin, new controller or note number, new channel
{ 
  bankTrue = true;

  bankPin = bPin;
  pinMode(bankPin, INPUT_PULLUP);
  newNote = newN; 
  newChannel = newC;
}

void DigitalLatch::detachBank()
{
  bankTrue = false;
  pinMode(bankPin, INPUT);
}


