#!/usr/bin/python3

import os
import shutil
from os.path import join, getsize, normpath, basename, splitext, relpath
import sys

script_dir = os.path.dirname(os.path.realpath(__file__))
src_dir = join(script_dir, "../src")

template = """\
#ifdef TEST_COMPILE_ALL_HEADERS_SEPARATELY
#include "{}"
#endif
"""


def main():
    for root, dirs, files in os.walk(src_dir):
        for f in files:
            name, ext = splitext(f)
            if ext == '.cpp':
                with open(join(root, f), 'r+') as cpp_file:
                    if not 'TEST_COMPILE_ALL_HEADERS_SEPARATELY' in \
                        cpp_file.read():
                        print(
                            relpath(normpath(join(root, f)),
                                    normpath(src_dir)))


if __name__ == "__main__":
    main()
