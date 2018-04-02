## Adding custom bitmaps to display

1. Use [GIMP](https://www.gimp.org/) to create a small black-and-white image, and export it as an X BitMap (`.xbm`) file.
2. Copy the XBM file to the `./XBM` subfolder of this folder. The filename can only contain alphanumeric characters and underscores, and cannot start with a number.
3. Run the Python script `./Scripts/XBM-export.py` to convert the XBM file to a format that the Arduino can read (`.axbm`) and to create an `XBitmap` from it.
4. (Optional) Run Doxygen again (in `doc/Doxygen`) to include the new icon in the documentation.