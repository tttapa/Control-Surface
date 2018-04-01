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

doxygendir = os.path.realpath(os.path.join(dirnm, '../../../doc/Doxygen/html'))
print(doxygendir)

for filename in sorted(os.listdir(dirnm)):
    if filename.endswith(".xbm"):
        print(filename)
        with open(os.path.join(dirnm, filename), 'r') as file:
            contents = file.read()
        identifier = os.path.splitext(filename)[0]
        contents = re.sub(r'(#define )[a-zA-Z_][a-zA-Z0-9_]*((?:(_width)|(_height)) +\d+)',r'\1{}\2'.format(identifier),contents)
        contents = re.sub(r'static unsigned char [a-zA-Z_][a-zA-Z0-9_]*(_bits\[\] *= *{ *(?:0x[0-9a-fA-F]{2}, *)*}?;?) *',r'static const PROGMEM uint8_t {}\1'.format(identifier),contents)
        m = re.search(r'#define [a-zA-Z\d_]+_width\s+(?P<width>\d+)[\r\n\s]+#define [a-zA-Z\d_]+_height\s+(?P<height>\d+)', contents)
        if m:
            width, height = m.group('width', 'height')
            print(width, height)
            m = re.search(r'{[\r\n\s]*((?:0x[a-fA-F0-9]{2},?[\r\n\s]*)+)}', contents)
            if m:
                data = m.group(1)
                # print(data)
                data = re.sub(r'[\r\n\s]|(?:0x)','', data)
                data = re.sub(r',',' ', data)
                bytedata = bytearray.fromhex(data)
#                bytedata = list(bytedata)
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
                        # print('({}, {})'.format(row, col),'index = {}, mask = {:08b}, value = 0x{:02x} = {:08b}, color = {}'.format(index, mask, bytedata[index], bytedata[index], color//255))
                        mask <<= 1
                        if mask & 0xFF == 0:
                            mask = 1
                            index += 1
                    imglist.append(rowlist)
                # print(imglist)
                png.from_array(imglist, 'L').save(os.path.join(doxygendir, identifier+'.png'))
                bitmaps.append((identifier,width,height))

                with open(os.path.join(dirnm, identifier+'.arduinoxbm'), 'w') as arduinoXBM:
                    arduinoXBM.write(contents)

XBitmaps = ''
for bm in bitmaps:
    id = bm[0]
    width = bm[1]
    height = bm[2]
    XBitmaps += '#include "{}"\r\n'.format(id + '.arduinoxbm') # TODO: multiline string
    XBitmaps += '/**\r\n * const XBitmap&emsp;<b>{}</b>&emsp;({}px × {}px)\r\n * <img src="{}" alt="" style="image-rendering:pixelated;image-rendering:-moz-crisp-edges;width:{}em;border:1px solid #ccc">\r\n */\r\n'.format(id, width, height, id + '.png', float(width)*0.75)
    XBitmaps += 'MakeXBitmap({});\r\n\r\n'.format(id)

with open(os.path.join(dirnm, 'XBitmaps.template'), 'r') as template:
    XBitmaps = re.sub(r':contents', XBitmaps, template.read())
    # print(XBitmaps)

with open(os.path.join(dirnm, 'XBitmaps.h'), 'w') as XBitmapsFile:
    XBitmapsFile.write(XBitmaps)
