#!/usr/bin/python3

import os, shutil
from os.path import join, getsize, normpath, basename, splitext, relpath
import sys

script_dir = os.path.dirname(os.path.realpath(__file__))
src_dir = join(script_dir, "../src")

def main():
    for root, dirs, files in os.walk(src_dir):
        for f in files:
            name, ext = splitext(f)
            print(name)
            if (ext == '.hpp' or ext == '.h') \
                and (not (name + '.cpp') in files) \
                and (not (name + '.c') in files):
                impl_ext = '.cpp' if ext == '.hpp' else '.c'
                with open(join(root, name + impl_ext), 'w') as cpp_file:
                    cpp_file.write('#include "{}"'.format(f))

if __name__=="__main__":
    main()
