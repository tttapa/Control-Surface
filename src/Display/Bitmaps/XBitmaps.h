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

#include "mute_10B.axbm"
/**
 * const XBitmap&emsp;<b>mute_10B</b>&emsp;(10px × 10px)
 * <img src="mute_10B.png" alt="mute_10B" class="xbm" style="width:7.5em;">
 */
MakeXBitmap(mute_10B);

#include "mute_14B.axbm"
/**
 * const XBitmap&emsp;<b>mute_14B</b>&emsp;(14px × 14px)
 * <img src="mute_14B.png" alt="mute_14B" class="xbm" style="width:10.5em;">
 */
MakeXBitmap(mute_14B);

#include "mute_7.axbm"
/**
 * const XBitmap&emsp;<b>mute_7</b>&emsp;(7px × 7px)
 * <img src="mute_7.png" alt="mute_7" class="xbm" style="width:5.25em;">
 */
MakeXBitmap(mute_7);

#include "play_10x9.axbm"
/**
 * const XBitmap&emsp;<b>play_10x9</b>&emsp;(10px × 9px)
 * <img src="play_10x9.png" alt="play_10x9" class="xbm" style="width:7.5em;">
 */
MakeXBitmap(play_10x9);

#include "play_7.axbm"
/**
 * const XBitmap&emsp;<b>play_7</b>&emsp;(7px × 7px)
 * <img src="play_7.png" alt="play_7" class="xbm" style="width:5.25em;">
 */
MakeXBitmap(play_7);

#include "play_8x7.axbm"
/**
 * const XBitmap&emsp;<b>play_8x7</b>&emsp;(8px × 7px)
 * <img src="play_8x7.png" alt="play_8x7" class="xbm" style="width:6.0em;">
 */
MakeXBitmap(play_8x7);

#include "rec_rdy_10B.axbm"
/**
 * const XBitmap&emsp;<b>rec_rdy_10B</b>&emsp;(10px × 10px)
 * <img src="rec_rdy_10B.png" alt="rec_rdy_10B" class="xbm" style="width:7.5em;">
 */
MakeXBitmap(rec_rdy_10B);

#include "rec_rdy_14B.axbm"
/**
 * const XBitmap&emsp;<b>rec_rdy_14B</b>&emsp;(14px × 14px)
 * <img src="rec_rdy_14B.png" alt="rec_rdy_14B" class="xbm" style="width:10.5em;">
 */
MakeXBitmap(rec_rdy_14B);

#include "rec_rdy_7.axbm"
/**
 * const XBitmap&emsp;<b>rec_rdy_7</b>&emsp;(7px × 7px)
 * <img src="rec_rdy_7.png" alt="rec_rdy_7" class="xbm" style="width:5.25em;">
 */
MakeXBitmap(rec_rdy_7);

#include "record_7.axbm"
/**
 * const XBitmap&emsp;<b>record_7</b>&emsp;(7px × 7px)
 * <img src="record_7.png" alt="record_7" class="xbm" style="width:5.25em;">
 */
MakeXBitmap(record_7);

#include "record_9.axbm"
/**
 * const XBitmap&emsp;<b>record_9</b>&emsp;(9px × 9px)
 * <img src="record_9.png" alt="record_9" class="xbm" style="width:6.75em;">
 */
MakeXBitmap(record_9);

#include "solo_10B.axbm"
/**
 * const XBitmap&emsp;<b>solo_10B</b>&emsp;(10px × 10px)
 * <img src="solo_10B.png" alt="solo_10B" class="xbm" style="width:7.5em;">
 */
MakeXBitmap(solo_10B);

#include "solo_14B.axbm"
/**
 * const XBitmap&emsp;<b>solo_14B</b>&emsp;(14px × 14px)
 * <img src="solo_14B.png" alt="solo_14B" class="xbm" style="width:10.5em;">
 */
MakeXBitmap(solo_14B);

#include "solo_7.axbm"
/**
 * const XBitmap&emsp;<b>solo_7</b>&emsp;(7px × 7px)
 * <img src="solo_7.png" alt="solo_7" class="xbm" style="width:5.25em;">
 */
MakeXBitmap(solo_7);

};

#endif // XBITMAPS_H
