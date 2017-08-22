/*
This is an example of the midi functions of the 'MIDI_controller' library.
Connect potentiometers to the appropriate analog inputs (lines 62-68).
Connect a switch to pin 11, and the other pin of the switch to the ground, internall pull-up resistor will be used.
Connect the USB MIDI output of the Arduino to a software synth or a hardware Roland synthesiser, using a tool like QjackCtl.
When the switch on pin 11 is turned on, this program should play a piece of the song 'Intoxicated' by Martin Solveig. (I do not own this song, obviously).

Volume1 controls the volume of the piano, Volume2 the volume of the bass line, and Volume10 the volume of the bass drum.
Chorus1 controls the chorus effect of the piano, Chorus2 controls the chorus effect on the bass. I think this is specific to Roland synthesisers. It doesn't work with software synths.
Reverb1 controls the reverb effect of the piano, Reverb2 controls the reverb effect on the bass (if you uncomment lines 64 and 85).
I think this is specific to Roland synthesisers. It doesn't work with software synths.

If you are using a Teensy, make sure you have the USB type set to MIDI.
If you are using an Arduino Uno or Mega, use the HIDUINO firmware for the ATmega16U2.


Written by tttapa, 21/08/2015
https://github.com/tttapa/MIDI_controller
*/

#include <MIDI_controller.h>


const int on[8][32]  = { // the array with note on events, 8 bars of 4 beats
{0,0,77,0,  77,0,0,76,  0,0,76,0,  76,0,0,76,    0,0,76,0,  76,0,0,79,  0,0,79,0,  76,0,0,76}, //piano
{0,0,72,0,  72,0,0,72,  0,0,72,0,  72,0,0,72,    0,0,72,0,  72,0,0,74,  0,0,74,0,  72,0,0,72}, //piano
{0,0,69,0,  69,0,0,67,  0,0,67,0,  67,0,0,67,    0,0,67,0,  67,0,0,72,  0,0,72,0,  67,0,0,67}, //piano
{0,0,65,0,  65,0,0,64,  0,0,64,0,  64,0,0,64,    0,0,64,0,  64,0,0,64,  0,0,64,0,  64,0,0,64}, //piano
{0,0,53,0,  53,0,0,48,  0,0,48,0,  48,0,0,57,    0,0,57,0,  57,0,0,55,  0,0,55,0,  48,0,0,48}, //piano
{0,0,41,0,  41,0,0,36,  0,0,36,0,  36,0,0,45,    0,0,45,0,  45,0,0,43,  0,0,43,0,  36,0,0,36}, //piano
{0,0,41,0,  41,0,0,36,  0,0,36,0,  36,0,0,45,    0,0,45,0,  45,0,0,43,  0,0,43,0,  36,0,0,36}, //bass
{0,0,0,0,   36,0,0,0,   0,0,0,0,   36,0,0,0,     0,0,0,0,   36,0,0,0,   0,0,0,0,   36,0,0,0 }  //bass drum 
};

const int off[8][32] = { // the array with note off events, 8 bars of 4 beats
{76,0,0,77,  0,0,0,77,   76,0,0,76,  0,0,0,76,    76,0,0,76,  0,0,0,76,  79,0,0,79,  0,0,0,76}, //piano
{72,0,0,72,  0,0,0,72,   72,0,0,72,  0,0,0,72,    72,0,0,72,  0,0,0,72,  74,0,0,74,  0,0,0,72}, //piano
{67,0,0,69,  0,0,0,69,   67,0,0,67,  0,0,0,67,    67,0,0,67,  0,0,0,67,  72,0,0,72,  0,0,0,67}, //piano
{64,0,0,65,  0,0,0,65,   65,0,0,64,  0,0,0,64,    64,0,0,64,  0,0,0,64,  64,0,0,64,  0,0,0,64}, //piano
{48,0,0,53,  0,0,0,53,   48,0,0,48,  0,0,0,48,    57,0,0,57,  0,0,0,57,  55,0,0,55,  0,0,0,48}, //piano
{36,0,0,41,  0,0,0,41,   36,0,0,36,  0,0,0,36,    45,0,0,45,  0,0,0,45,  43,0,0,43,  0,0,0,36}, //piano
{36,0,0,41,  0,0,0,41,   36,0,0,36,  0,0,0,36,    45,0,0,45,  0,0,0,45,  43,0,0,43,  0,0,0,36}, //bass
{36,0,0,0,   0,0,0,0,    36,0,0,0,   0,0,0,0,     36,0,0,0,   36,0,0,0,  0,0,0,0,    0,0,0,0 }  //bass drum
};

const int vel[8] = // the velocities per line of notes
{90,80,80,80,80,80,110,90};

const int chan[8] = // the channel per line of notes
{1,1,1,1,1,1,2,10};


boolean allOffSent = false;

const static byte chorus = 93;
const static byte reverb = 91;
const static byte channelVolume = 7;

//________________________________________________________________________________________________________________________________

Analog chorus1(A8, chorus, 1, 128);  // Change this when using Arduino Uno.
Analog chorus2(A9, chorus, 2, 128);  // Change this when using Arduino Uno.
Analog reverb1(A10, reverb, 1, 128); // Change this when using Arduino Uno.
//Analog reverb2(A11, reverb, 2, 128);
Analog volume1(A0, channelVolume, 1, 128);
Analog volume2(A1, channelVolume, 2, 128);
Analog volume10(A2, channelVolume, 10, 128);

//_______________________________________________________________________________________________________________________________

void setup(){
  USBMidiController.blink(LED_BUILTIN);  // flash the built-in LED (pin 13 on most boards) on every message
  USBMidiController.setDelay(0);  // wait 15 ms after each message not to flood the connection 
  // We don't want a delay after each message, because otherwise two notes that should play simultaneously will have this delay between them, resulting in a strange, unwanted effect.
  USBMidiController.begin();  // Initialise the USB MIDI connection
  pinMode(11,INPUT_PULLUP);
  while(digitalRead(11) == 1); // Wait untill the switch is turned on.
}

//_______________________________________________________________________________________________________________________________

void loop(){
  sendMIDI(PROGRAM_CHANGE,2,33); // channel 2 is now instrument 33 (bass)
  for(int i = 0; i<32; i++){
    chorus1.refresh();
    chorus2.refresh();
    reverb1.refresh();
    //reverb2.refresh();
    volume1.refresh();
    volume2.refresh();
    volume10.refresh();
    for(int j = 0; j<8; j++){
      if(off[j][i] != 0){
        sendMIDI(NOTE_OFF, chan[j], off[j][i], 0);
      }
      while(digitalRead(11) == 1){ // wait for the switch to be on
        if(!allOffSent){
          sendMIDI(CC, 1, 123, 0);  // all notes off channel 1
          sendMIDI(CC, 2, 123, 0);  // all notes off channel 2
          sendMIDI(CC, 10, 123, 0); // all notes off channel 10
          allOffSent = true;
        }
      }
      allOffSent = false;
        
      if(on[j][i] != 0){
        sendMIDI(NOTE_ON, chan[j], on[j][i], vel[j]);
      }
    }
    delay(115);
  }
}