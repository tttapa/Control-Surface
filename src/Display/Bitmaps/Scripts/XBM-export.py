#!/usr/bin/env python3

"""
Converts XBM files to header files that 
can be included in Arduino source code. 
Bitmaps are saved to program memory (PROGMEM).

The filename (without extension) is used 
as the identifier for the bitmap.

A header file is created that includes all 
these bitmaps (as a XBitmap struct), including
Doxygen documentation.
All XBM images are converted to PNG images
and saved to the Doxygen HTML folder, to be
displayed in the documentation.
"""

# TODO: make this code readable.

import os
import re
import png

bitmaps = []

dirnm = os.path.dirname(os.path.realpath(__file__))

inputdir = os.path.realpath(os.path.join(dirnm,  '../XBM'))
outputdir = os.path.realpath(os.path.join(dirnm, '..'))
# pngoutputdir = os.path.realpath(os.path.join(dirnm, '../PNG'))
pngoutputdir = os.path.realpath(os.path.join(dirnm, '../../../../doc/Doxygen/html'))

def XBM2PNG(bytedata):
    index = 0
    mask = 1
    imglist = []
    for row in range(int(height)):
        if mask != 1:
            index += 1
            mask = 1
        rowlist = []
        for col in range(int(width)):
            color = 255*int(not bool(bytedata[index] & mask))
            rowlist.append(color)
            mask <<= 1
            if mask & 0xFF == 0:
                mask = 1
                index += 1
        imglist.append(rowlist)
    return png.from_array(imglist, 'L')

for filename in sorted(os.listdir(inputdir)):
    if filename.endswith(".xbm"):
        print(filename)
        with open(os.path.join(inputdir, filename), 'r') as file:
            contents = file.read()
        identifier = os.path.splitext(filename)[0]
        contents = re.sub(r'(#define )[a-zA-Z_][a-zA-Z0-9_]*((?:(_width)|(_height)) +\d+)',
                          r'\1{}\2'.format(identifier),
                          contents)
        contents = re.sub(r'static unsigned char [a-zA-Z_][a-zA-Z0-9_]*(_bits\[\] *= *{ *(?:0x[0-9a-fA-F]{2}, *)*}?;?) *',
                          r'static const PROGMEM uint8_t {}\1'.format(identifier),  # @todo  should these be static?
                          contents)
        m = re.search(r'#define [a-zA-Z\d_]+_width\s+(?P<width>\d+)[\r\n\s]+#define [a-zA-Z\d_]+_height\s+(?P<height>\d+)', 
                      contents)
        if m:
            width, height = m.group('width', 'height')
            print('\t({}px × {}px)'.format(width, height))
            m = re.search(r'{[\r\n\s]*((?:0x[a-fA-F0-9]{2},?[\r\n\s]*)+)}', contents)
            if m:
                data = m.group(1)
                data = re.sub(r'[\r\n\s]|(?:0x)','', data)
                data = re.sub(r',',' ', data)
                bytedata = bytearray.fromhex(data)
                PNG = XBM2PNG(bytedata)
                PNG.save(os.path.join(pngoutputdir, identifier+'.png'))
                bitmaps.append((identifier,width,height))

                with open(os.path.join(outputdir, identifier+'.axbm'), 'w') as axbm:
                    axbm.write(contents)
            else:
                print('Warning: invalid XBM data')
        else:
            print('Warning: invalid XBM width or height data')
        print()

XBitmaps = ''
for bm in bitmaps:
    id = bm[0]
    width = bm[1]
    height = bm[2]

    XBitmaps += \
"""#include "{id}.axbm"
/**
 * const XBitmap&emsp;<b>{id}</b>&emsp;({width}px × {height}px)
 * <img src="{id}.png" alt="{id}" class="xbm" style="width:{pngwidth}em;">
 */
MakeXBitmap({id});

""".format(id=id, width=width, height=height, pngwidth=(float(width)*0.75))

with open(os.path.join(outputdir, 'XBitmaps.template'), 'r') as template:
    XBitmaps = re.sub(r':contents', XBitmaps, template.read())

with open(os.path.join(outputdir, 'XBitmaps.h'), 'w') as XBitmapsFile:
    XBitmapsFile.write(XBitmaps)
