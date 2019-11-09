doc: examples xbm
	doxygen

xbm: 
	./../../src/Display/Bitmaps/Scripts/XBM-export.py

examples:
	./Scripts/examples.py

export: doc
	cp -a html/. ../../../tttapa.github.io/Control-Surface/Doc/Doxygen
	