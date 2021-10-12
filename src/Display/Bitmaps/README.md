## Adding custom bitmaps to display {#md_pages_xbm}

1. Use [GIMP](https://www.gimp.org/) to create a small black-and-white image, and export it as an X BitMap (`.xbm`) file.
2. Copy the XBM file to the `src/Display/Bitmaps/XBM` folder. The filename can only contain alphanumeric characters and underscores, and cannot start with a number.
3. (Optional) Install the Python PNG library (`requirements.txt`).
4. Run the Python script `src/Display/Bitmaps/Scripts/XBM-export.py` to convert the XBM file to a format that the Arduino can read (`.axbm`) and to create an `XBitmap` from it.
5. (Optional) Run Doxygen again (in the `doxygen` folder) to include the new icon in the documentation.
6. You can now use the bitmap in your Arduino sketches, using the original file name (without `.xbm`).

```sh
mkdir -p docs/Doxygen
cd docs/Doxygen
python3 -m pip install -r ../../src/Display/Bitmaps/Scripts/requirements.txt
python3 ../../src/Display/Bitmaps/Scripts/XBM-export.py
cd ../../doxygen
doxygen
```