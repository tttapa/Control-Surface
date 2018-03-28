#include "../Display.hpp"

namespace XBM
{

#define MakeXBitmap(name) \
    XBitmap name = \
         { name##_width, name##_height, name##_bits }

// =========================================================================== //

#define mute10_width 10
#define mute10_height 10
const static uint8_t mute10_bits[] PROGMEM = {
    0x01, 0x00, 0x89, 0x00, 0xd9, 0x00, 0xa9, 0x00, 0x89, 0x00, 0x89, 0x00,
    0x89, 0x00, 0x01, 0x00, 0x01, 0x00, 0xff, 0x03};

MakeXBitmap(mute10);

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //

#define play7_width 7
#define play7_height 7
const static uint8_t play7_bits[] PROGMEM = {
    0x01, 0x07, 0x1f, 0x7f, 0x1f, 0x07, 0x01};

MakeXBitmap(play7);

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //

#define record7_width 7
#define record7_height 7
const static uint8_t record7_bits[] PROGMEM = {
    0x1c, 0x3e, 0x7f, 0x7f, 0x7f, 0x3e, 0x1c};

MakeXBitmap(record7);

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //

#define recordRdy10_width 10
#define recordRdy10_height 10
const static uint8_t recordRdy10_bits[] PROGMEM = {
    0x01, 0x00, 0x79, 0x00, 0x89, 0x00, 0x89, 0x00, 0x79, 0x00, 0x29, 0x00,
    0x49, 0x00, 0x89, 0x00, 0x01, 0x00, 0xff, 0x03};

MakeXBitmap(recordRdy10);

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //

#define solo7_width 7
#define solo7_height 7
const static uint8_t solo7_bits[] = {
    0x1c, 0x22, 0x02, 0x1c, 0x20, 0x22, 0x1c};

MakeXBitmap(solo7);

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //

#define solo10_width 10
#define solo10_height 10
const static uint8_t solo10_bits[] PROGMEM = {
    0x01, 0x00, 0x71, 0x00, 0x89, 0x00, 0x09, 0x00, 0x71, 0x00, 0x81, 0x00,
    0x89, 0x00, 0x71, 0x00, 0x01, 0x00, 0xff, 0x03};

MakeXBitmap(solo10);

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
};