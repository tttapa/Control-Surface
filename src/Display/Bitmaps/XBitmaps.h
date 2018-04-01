#ifndef XBITMAPS_H
#define XBITMAPS_H

#include "../Display.hpp"


struct XBitmap
{
  XBitmap(uint16_t width, uint16_t height, const uint8_t bits[])
      : width(width), height(height), bits(bits) {}
  uint16_t width;
  uint16_t height;
  const uint8_t *bits;
};

namespace XBM
{

#define MakeXBitmap(name) \
    const XBitmap name = \
         { name##_width, name##_height, name##_bits }

// =========================================================================== //

#include "mute_10B.arduinoxbm"
/**
 * XBitmap&emsp;<b>mute_10B</b>&emsp;(10px × 10px)
 * <img src="mute_10B.png" alt="" style="image-rendering:pixelated;margin-right:100%;width:7.5em;border:1px solid #ccc">
 */
MakeXBitmap(mute_10B);

#include "mute_14B.arduinoxbm"
/**
 * XBitmap&emsp;<b>mute_14B</b>&emsp;(14px × 14px)
 * <img src="mute_14B.png" alt="" style="image-rendering:pixelated;margin-right:100%;width:10.5em;border:1px solid #ccc">
 */
MakeXBitmap(mute_14B);

#include "mute_7.arduinoxbm"
/**
 * XBitmap&emsp;<b>mute_7</b>&emsp;(7px × 7px)
 * <img src="mute_7.png" alt="" style="image-rendering:pixelated;margin-right:100%;width:5.25em;border:1px solid #ccc">
 */
MakeXBitmap(mute_7);

#include "play_10x9.arduinoxbm"
/**
 * XBitmap&emsp;<b>play_10x9</b>&emsp;(10px × 9px)
 * <img src="play_10x9.png" alt="" style="image-rendering:pixelated;margin-right:100%;width:7.5em;border:1px solid #ccc">
 */
MakeXBitmap(play_10x9);

#include "play_7.arduinoxbm"
/**
 * XBitmap&emsp;<b>play_7</b>&emsp;(7px × 7px)
 * <img src="play_7.png" alt="" style="image-rendering:pixelated;margin-right:100%;width:5.25em;border:1px solid #ccc">
 */
MakeXBitmap(play_7);

#include "play_8x7.arduinoxbm"
/**
 * XBitmap&emsp;<b>play_8x7</b>&emsp;(8px × 7px)
 * <img src="play_8x7.png" alt="" style="image-rendering:pixelated;margin-right:100%;width:6.0em;border:1px solid #ccc">
 */
MakeXBitmap(play_8x7);

#include "rec_rdy_10B.arduinoxbm"
/**
 * XBitmap&emsp;<b>rec_rdy_10B</b>&emsp;(10px × 10px)
 * <img src="rec_rdy_10B.png" alt="" style="image-rendering:pixelated;margin-right:100%;width:7.5em;border:1px solid #ccc">
 */
MakeXBitmap(rec_rdy_10B);

#include "rec_rdy_14B.arduinoxbm"
/**
 * XBitmap&emsp;<b>rec_rdy_14B</b>&emsp;(14px × 14px)
 * <img src="rec_rdy_14B.png" alt="" style="image-rendering:pixelated;margin-right:100%;width:10.5em;border:1px solid #ccc">
 */
MakeXBitmap(rec_rdy_14B);

#include "rec_rdy_7.arduinoxbm"
/**
 * XBitmap&emsp;<b>rec_rdy_7</b>&emsp;(7px × 7px)
 * <img src="rec_rdy_7.png" alt="" style="image-rendering:pixelated;margin-right:100%;width:5.25em;border:1px solid #ccc">
 */
MakeXBitmap(rec_rdy_7);

#include "record_7.arduinoxbm"
/**
 * XBitmap&emsp;<b>record_7</b>&emsp;(7px × 7px)
 * <img src="record_7.png" alt="" style="image-rendering:pixelated;margin-right:100%;width:5.25em;border:1px solid #ccc">
 */
MakeXBitmap(record_7);

#include "record_9.arduinoxbm"
/**
 * XBitmap&emsp;<b>record_9</b>&emsp;(9px × 9px)
 * <img src="record_9.png" alt="" style="image-rendering:pixelated;margin-right:100%;width:6.75em;border:1px solid #ccc">
 */
MakeXBitmap(record_9);

#include "solo_10B.arduinoxbm"
/**
 * XBitmap&emsp;<b>solo_10B</b>&emsp;(10px × 10px)
 * <img src="solo_10B.png" alt="" style="image-rendering:pixelated;margin-right:100%;width:7.5em;border:1px solid #ccc">
 */
MakeXBitmap(solo_10B);

#include "solo_14B.arduinoxbm"
/**
 * XBitmap&emsp;<b>solo_14B</b>&emsp;(14px × 14px)
 * <img src="solo_14B.png" alt="" style="image-rendering:pixelated;margin-right:100%;width:10.5em;border:1px solid #ccc">
 */
MakeXBitmap(solo_14B);

#include "solo_7.arduinoxbm"
/**
 * XBitmap&emsp;<b>solo_7</b>&emsp;(7px × 7px)
 * <img src="solo_7.png" alt="" style="image-rendering:pixelated;margin-right:100%;width:5.25em;border:1px solid #ccc">
 */
MakeXBitmap(solo_7);

};

#endif // XBITMAPS_H
