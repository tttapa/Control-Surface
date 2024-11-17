/** 
 * @brief   This is an example to demonstrate the use of a ButtonMatrix.
 * 
 * ### Connections
 *  
 *  - A0-A3: The row pins of the button matrix (with the cathodes of the diodes)
 *  - 2-13:  The column pins of the button matrix
 *  - A4-A5: Two push buttons (to ground) to transpose the keyboard
 * 
 * The internal pull-up resistors will be used.
 * 
 * ### Behavior
 * 
 * The keys of the keyboard should send note on/off events when 
 * pressed/released.  
 * The notes can be transposed by pressing the buttons connected to pins A4-A5.
 * 
 * Written by Pieter P, 27-01-2019  
 * https://github.com/tttapa/Control-Surface
 */

#include <Control_Surface.h>

USBMIDI_Interface midi;

// A transposer that can transpose the keyboard one octave lower or higher
Transposer<-12, +12> transposer;

// Buttons to transpose the keyboard
IncrementDecrementSelector<transposer.N> transposebuttons = {
    transposer,
    {A4, A5},
    Wrap::NoWrap,
};

using namespace MIDI_Notes;
Bankable::NoteButtonMatrix<4, 12> keyboard = {
    transposer,
    {A0, A1, A2, A3},                         // Row pins (driven Lo-Z LOW!)
    {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13}, // Column pins (input-pullup)
    // All notes
    {{
        {
            note(C, 2),
            note(Db, 2),
            note(D, 2),
            note(Eb, 2),
            note(E, 2),
            note(F, 2),
            note(Gb, 2),
            note(G, 2),
            note(Ab, 2),
            note(A, 2),
            note(Bb, 2),
            note(B, 2),
        },
        {
            note(C, 3),
            note(Db, 3),
            note(D, 3),
            note(Eb, 3),
            note(E, 3),
            note(F, 3),
            note(Gb, 3),
            note(G, 3),
            note(Ab, 3),
            note(A, 3),
            note(Bb, 3),
            note(B, 3),
        },
        {
            note(C, 4),
            note(Db, 4),
            note(D, 4),
            note(Eb, 4),
            note(E, 4),
            note(F, 4),
            note(Gb, 4),
            note(G, 4),
            note(Ab, 4),
            note(A, 4),
            note(Bb, 4),
            note(B, 4),
        },
        {
            note(C, 5),
            note(Db, 5),
            note(D, 5),
            note(Eb, 5),
            note(E, 5),
            note(F, 5),
            note(Gb, 5),
            note(G, 5),
            note(Ab, 5),
            note(A, 5),
            note(Bb, 5),
            note(B, 5),
        },
    }},
};

void setup() { Control_Surface.begin(); }

void loop() { Control_Surface.loop(); }