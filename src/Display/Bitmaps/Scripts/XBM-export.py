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
All XBM images are converted to PNG images, to be
displayed in the documentation.
The PNG images are exported to the current working directory.
"""

# TODO: make this code readable.

import os
import re
try:
    import png
except ImportError as e:
    png = None 
    print("png not found, not exporting PNG images:", e)
    print("(See requirements.txt)")

bitmaps = []

dirnm = os.path.dirname(os.path.realpath(__file__))

inputdir = os.path.realpath(os.path.join(dirnm,  '../XBM'))
outputdir = os.path.realpath(os.path.join(dirnm, '..'))
# pngoutputdir = os.path.realpath(os.path.join(dirnm, '../PNG'))
pngoutputdir = os.path.join(os.getcwd(), 'xbm')

os.makedirs(pngoutputdir, exist_ok=True)


def XBM2PNG(bytedata, width, height):
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


def sanitize_identifier(id: str) -> str:
    newid = re.sub(r'[^a-zA-Z0-9_]', r'_', id)
    assert(not newid.isdecimal())
    while newid[0].isdecimal():
        newid = newid[1:]
    if (newid != id):
        print('Warning: changing identifier from `'+id+'` to `'+newid+'`')
    return id


for filename in sorted(os.listdir(inputdir)):
    if filename.endswith(".xbm"):
        print(filename)
        # Read the contents of the XBM file
        with open(os.path.join(inputdir, filename), 'r') as file:
            contents = file.read()
        # Use the file name as the identifier
        identifier = sanitize_identifier(os.path.splitext(filename)[0])
        # Replace the existing identifier for width and height,
        # and change the #defines to constants
        contents = re.sub(r'#define [a-zA-Z_][a-zA-Z0-9_]*((?:_width)|(?:_height)) +(\d+)',
                          r'constexpr size_t {}\1 = \2;'.format(identifier),
                          contents)
        # Replace the existing identifier for the data array,
        # change the type from unsigned char to uint8_t, 
        # and store it in PROGMEM
        contents = re.sub(r'static (?:unsigned )?char [a-zA-Z_][a-zA-Z0-9_]*(_bits\[\] *= *{ *(?:0x[0-9a-fA-F]{2}, *)*}?;?) *',
                          r'static const PROGMEM uint8_t {}\1'.format(identifier), contents)
        # Extract the width and height
        m = re.search(r'.*_width\s*=\s*(?P<width>\d+)\s*;[\r\n\s]+.*_height\s*=\s*(?P<height>\d+)',
                      contents)
        if not m:
            print('Warning: invalid XBM width or height data')
            continue
        width, height = m.group('width', 'height')
        print('\t({}px × {}px)'.format(width, height))
        # Extract the image data
        m = re.search(
            r'{[\r\n\s]*((?:0x[a-fA-F0-9]{2},?[\r\n\s]*)+)}', contents)
        if not m:
            print('Warning: invalid XBM data')
            continue
        # Convert the data from "0xff, 0xff, 0xff" to "ff ff ff"
        data = m.group(1)
        data = re.sub(r'[\r\n\s]|(?:0x)', '', data)
        data = re.sub(r',', ' ', data)
        if png is not None:
            bytedata = bytearray.fromhex(data)
            # and then to a byte array that can be converted to PNG
            PNG = XBM2PNG(bytedata, width, height)
            # Save the PNG image for the documentation pages 
            # (browsers don't do XBM)
            PNG.save(os.path.join(pngoutputdir, identifier+'.png'))

        # Save the name, width and height of the bitmap
        bitmaps.append((identifier, width, height))
        # Save the bitmap source file that can be used with Arduino
        with open(os.path.join(outputdir, identifier+'.axbm'), 'w') as axbm:
            axbm.write('#ifndef DOXYGEN\n'+contents+'\n'+'#endif\n')
        print()

XBitmaps = """// Automatically generated file: do not edit

"""
for bm in bitmaps:
    id = bm[0]
    width = bm[1]
    height = bm[2]

    XBitmaps += \
        """#include "{id}.axbm"
/**
 * XBitmap&emsp;<b>{id}</b>&emsp;({width}px × {height}px)
 * <img src="xbm/{id}.png" alt="{id}" class="xbm" style="width:{pngwidth}em;">
 */
const XBitmap {id} = {{ {id}_width, {id}_height, {id}_bits }};

""".format(id=id, width=width, height=height, pngwidth=float(width)*0.75)

with open(os.path.join(outputdir, 'XBitmaps.template'), 'r') as template:
    XBitmaps = re.sub(r':contents', XBitmaps, template.read())

with open(os.path.join(outputdir, 'XBitmaps.hpp'), 'w') as XBitmapsFile:
    XBitmapsFile.write(XBitmaps)
