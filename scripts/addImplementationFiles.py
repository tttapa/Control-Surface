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
                    num_lines = sum(1 for line in cpp_file)
                    if num_lines > 1 and num_lines < 5:
                        print(num_lines)
                        print(normpath(join(root, f)))

            # if ext == '.cpp':
            #     with open(join(root, f), 'r+') as cpp_file:
            #         num_lines = sum(1 for line in cpp_file)
            #         print(num_lines)
            #         cpp_file.seek(0)
            #         content = cpp_file.read()
            #         if num_lines == 1:
            #             cpp_file.seek(0)
            #             cpp_file.write(
            #                 '#ifdef TEST_COMPILE_ALL_HEADERS_SEPARATELY\n')
            #             cpp_file.write(content)
            #             cpp_file.write('\n#endif')

            if (ext == '.hpp' or ext == '.h') \
                and (not (name + '.cpp') in files) \
                and (not (name + '.c') in files) \
                and (basename(root) != 'Doxygen'):
                impl_ext = '.cpp' if ext == '.hpp' else '.c'
                with open(join(root, name + impl_ext), 'w') as cpp_file:
                    cpp_file.write(template.format(f))


if __name__ == "__main__":
    main()
