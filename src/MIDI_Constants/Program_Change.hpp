#pragma once

#include <Settings/NamespaceSettings.hpp>
#include <stdint.h>

BEGIN_CS_NAMESPACE

/// @addtogroup    MIDIConstants
/// @{

/**
 * @brief   Names and values for all MIDI Control Change Controller Numbers.
 */
namespace MIDI_PC {

constexpr uint8_t Acoustic_Grand_Piano = 0;   // Acoustic Grand Piano
constexpr uint8_t Bright_Acoustic_Piano = 1;  // Bright Acoustic Piano
constexpr uint8_t Electric_Grand_Piano = 2;   // Electric Grand Piano
constexpr uint8_t HonkyTonk_Piano = 3;        // Honky-tonk Piano
constexpr uint8_t Electric_Piano_1 = 4;       // Electric Piano 1
constexpr uint8_t Electric_Piano_2 = 5;       // Electric Piano 2
constexpr uint8_t Harpsichord = 6;            // Harpsichord
constexpr uint8_t Clavi = 7;                  // Clavi
constexpr uint8_t Celesta = 8;                // Celesta
constexpr uint8_t Glockenspiel = 9;           // Glockenspiel
constexpr uint8_t Music_Box = 10;             // Music Box
constexpr uint8_t Vibraphone = 11;            // Vibraphone
constexpr uint8_t Marimba = 12;               // Marimba
constexpr uint8_t Xylophone = 13;             // Xylophone
constexpr uint8_t Tubular_Bells = 14;         // Tubular Bells
constexpr uint8_t Dulcimer = 15;              // Dulcimer
constexpr uint8_t Drawbar_Organ = 16;         // Drawbar Organ
constexpr uint8_t Percussive_Organ = 17;      // Percussive Organ
constexpr uint8_t Rock_Organ = 18;            // Rock Organ
constexpr uint8_t Church_Organ = 19;          // Church Organ
constexpr uint8_t Reed_Organ = 20;            // Reed Organ
constexpr uint8_t Accordion = 21;             // Accordion
constexpr uint8_t Harmonica = 22;             // Harmonica
constexpr uint8_t Tango_Accordion = 23;       // Tango Accordion
constexpr uint8_t Acoustic_Guitar_Nylon = 24; // Acoustic Guitar (nylon)
constexpr uint8_t Acoustic_Guitar_Steel = 25; // Acoustic Guitar (steel)
constexpr uint8_t Electric_Guitar_Jazz = 26;  // Electric Guitar (jazz)
constexpr uint8_t Electric_Guitar_Clean = 27; // Electric Guitar (clean)
constexpr uint8_t Electric_Guitar_Muted = 28; // Electric Guitar (muted)
constexpr uint8_t Overdriven_Guitar = 29;     // Overdriven Guitar
constexpr uint8_t Distortion_Guitar = 30;     // Distortion Guitar
constexpr uint8_t Guitar_harmonics = 31;      // Guitar harmonics
constexpr uint8_t Acoustic_Bass = 32;         // Acoustic Bass
constexpr uint8_t Electric_Bass_Finger = 33;  // Electric Bass (finger)
constexpr uint8_t Electric_Bass_Pick = 34;    // Electric Bass (pick)
constexpr uint8_t Fretless_Bass = 35;         // Fretless Bass
constexpr uint8_t Slap_Bass_1 = 36;           // Slap Bass 1
constexpr uint8_t Slap_Bass_2 = 37;           // Slap Bass 2
constexpr uint8_t Synth_Bass_1 = 38;          // Synth Bass 1
constexpr uint8_t Synth_Bass_2 = 39;          // Synth Bass 2
constexpr uint8_t Violin = 40;                // Violin
constexpr uint8_t Viola = 41;                 // Viola
constexpr uint8_t Cello = 42;                 // Cello
constexpr uint8_t Contrabass = 43;            // Contrabass
constexpr uint8_t Tremolo_Strings = 44;       // Tremolo Strings
constexpr uint8_t Pizzicato_Strings = 45;     // Pizzicato Strings
constexpr uint8_t Orchestral_Harp = 46;       // Orchestral Harp
constexpr uint8_t Timpani = 47;               // Timpani
constexpr uint8_t String_Ensemble_1 = 48;     // String Ensemble 1
constexpr uint8_t String_Ensemble_2 = 49;     // String Ensemble 2
constexpr uint8_t SynthStrings_1 = 50;        // SynthStrings 1
constexpr uint8_t SynthStrings_2 = 51;        // SynthStrings 2
constexpr uint8_t Choir_Aahs = 52;            // Choir Aahs
constexpr uint8_t Voice_Oohs = 53;            // Voice Oohs
constexpr uint8_t Synth_Voice = 54;           // Synth Voice
constexpr uint8_t Orchestra_Hit = 55;         // Orchestra Hit
constexpr uint8_t Trumpet = 56;               // Trumpet
constexpr uint8_t Trombone = 57;              // Trombone
constexpr uint8_t Tuba = 58;                  // Tuba
constexpr uint8_t Muted_Trumpet = 59;         // Muted Trumpet
constexpr uint8_t French_Horn = 60;           // French Horn
constexpr uint8_t Brass_Section = 61;         // Brass Section
constexpr uint8_t SynthBrass_1 = 62;          // SynthBrass 1
constexpr uint8_t SynthBrass_2 = 63;          // SynthBrass 2
constexpr uint8_t Soprano_Sax = 64;           // Soprano Sax
constexpr uint8_t Alto_Sax = 65;              // Alto Sax
constexpr uint8_t Tenor_Sax = 66;             // Tenor Sax
constexpr uint8_t Baritone_Sax = 67;          // Baritone Sax
constexpr uint8_t Oboe = 68;                  // Oboe
constexpr uint8_t English_Horn = 69;          // English Horn
constexpr uint8_t Bassoon = 70;               // Bassoon
constexpr uint8_t Clarinet = 71;              // Clarinet
constexpr uint8_t Piccolo = 72;               // Piccolo
constexpr uint8_t Flute = 73;                 // Flute
constexpr uint8_t Recorder = 74;              // Recorder
constexpr uint8_t Pan_Flute = 75;             // Pan Flute
constexpr uint8_t Blown_Bottle = 76;          // Blown Bottle
constexpr uint8_t Shakuhachi = 77;            // Shakuhachi
constexpr uint8_t Whistle = 78;               // Whistle
constexpr uint8_t Ocarina = 79;               // Ocarina
constexpr uint8_t Lead_1 = 80;                // Lead 1 (square)
constexpr uint8_t Lead_2 = 81;                // Lead 2 (sawtooth)
constexpr uint8_t Lead_3 = 82;                // Lead 3 (calliope)
constexpr uint8_t Lead_4 = 83;                // Lead 4 (chiff)
constexpr uint8_t Lead_5 = 84;                // Lead 5 (charang)
constexpr uint8_t Lead_6 = 85;                // Lead 6 (voice)
constexpr uint8_t Lead_7 = 86;                // Lead 7 (fifths)
constexpr uint8_t Lead_8 = 87;                // Lead 8 (bass + lead)
constexpr uint8_t Pad_1 = 88;                 // Pad 1 (new age)
constexpr uint8_t Pad_2 = 89;                 // Pad 2 (warm)
constexpr uint8_t Pad_3 = 90;                 // Pad 3 (polysynth)
constexpr uint8_t Pad_4 = 91;                 // Pad 4 (choir)
constexpr uint8_t Pad_5 = 92;                 // Pad 5 (bowed)
constexpr uint8_t Pad_6 = 93;                 // Pad 6 (metallic)
constexpr uint8_t Pad_7 = 94;                 // Pad 7 (halo)
constexpr uint8_t Pad_8 = 95;                 // Pad 8 (sweep)
constexpr uint8_t FX_1 = 96;                  // FX 1 (rain)
constexpr uint8_t FX_2 = 97;                  // FX 2 (soundtrack)
constexpr uint8_t FX_3 = 98;                  // FX 3 (crystal)
constexpr uint8_t FX_4 = 99;                  // FX 4 (atmosphere)
constexpr uint8_t FX_5 = 100;                 // FX 5 (brightness)
constexpr uint8_t FX_6 = 101;                 // FX 6 (goblins)
constexpr uint8_t FX_7 = 102;                 // FX 7 (echoes)
constexpr uint8_t FX_8 = 103;                 // FX 8 (sci-fi)
constexpr uint8_t Sitar = 104;                // Sitar
constexpr uint8_t Banjo = 105;                // Banjo
constexpr uint8_t Shamisen = 106;             // Shamisen
constexpr uint8_t Koto = 107;                 // Koto
constexpr uint8_t Kalimba = 108;              // Kalimba
constexpr uint8_t Bag_Pipe = 109;             // Bag pipe
constexpr uint8_t Fiddle = 110;               // Fiddle
constexpr uint8_t Shanai = 111;               // Shanai
constexpr uint8_t Tinkle_Bell = 112;          // Tinkle Bell
constexpr uint8_t Agogo = 113;                // Agogo
constexpr uint8_t Steel_Drums = 114;          // Steel Drums
constexpr uint8_t Woodblock = 115;            // Woodblock
constexpr uint8_t Taiko_Drum = 116;           // Taiko Drum
constexpr uint8_t Melodic_Tom = 117;          // Melodic Tom
constexpr uint8_t Synth_Drum = 118;           // Synth Drum
constexpr uint8_t Reverse_Cymbal = 119;       // Reverse Cymbal
constexpr uint8_t Guitar_Fret_Noise = 120;    // Guitar Fret Noise
constexpr uint8_t Breath_Noise = 121;         // Breath Noise
constexpr uint8_t Seashore = 122;             // Seashore
constexpr uint8_t Bird_Tweet = 123;           // Bird Tweet
constexpr uint8_t Telephone_Ring = 124;       // Telephone Ring
constexpr uint8_t Helicopter = 125;           // Helicopter
constexpr uint8_t Applause = 126;             // Applause
constexpr uint8_t Gunshot = 127;              // Gunshot

} // namespace MIDI_PC

/// @}

END_CS_NAMESPACE