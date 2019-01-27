#!/usr/bin/env python3

import re
import os
from os.path import join, realpath, basename

dirnm = os.path.dirname(os.path.realpath(__file__))
doxydir = join(dirnm, '..')

def stripQuotes(string):
    string = re.sub(r'^"','',string)
    string = re.sub(r'"$','',string)
    return string

with open(join(doxydir, 'Doxyfile')) as doxy:
    doxy_content = doxy.read()
    m = re.search(r'EXAMPLE_PATH\s*=\s*(.+)', doxy_content)
    if m:
        exampledir = m.group(1).split('" "')[0]
        exampledir = realpath(join(doxydir, stripQuotes(exampledir)))
        print("Example directory =", exampledir)
    else:
        print('Error: couldn\'t find EXAMPLE_PATH in Doxyfile')
        exit(1)
    m = re.search(r'INPUT\s*=\s*([^\\]+)(\s|\\)*', doxy_content)
    if m:
        lastInclude = m.group(1).split(" ")[-1]
        outputfile = realpath(join(doxydir, stripQuotes(lastInclude)))
        print("Output file =", outputfile)
    else:
        print('Error: couldn\'t find INPUT in Doxyfile')
        exit(1)

output = ""

for root, dirs, files in os.walk(exampledir):
    print('Scanning', root)
    for file in sorted(files):
        if file.endswith('.ino'):
            with open(join(root, file)) as example:
                example_content = example.read()
                s = example_content.split('/**',1)
                if len(s) > 1:
                    print('\t\033[0;32mFound documentation for', file, 
                        '\033[0m')
                    docstr = s[1].split('*/',1)[0]
                    output += "/**\r\n * @example   "
                    output += basename(file)
                    output += "\r\n *"
                    output += docstr
                    output += "*/\r\n\r\n"
                else:
                    print('\t\033[0;33mWarning: no documentation for', file,
                        '\033[0m')

with open(outputfile, 'w') as f:
    f.write(output)
