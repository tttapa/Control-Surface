#!/usr/bin/python3
"""
Script that gathers all keywords from the keywords.yml files in the src 
directory into the main keywords.txt file in the root folder.
"""

import os
import shutil
from os.path import join, normpath, dirname, splitext, relpath, realpath
import sys
import re

from yaml import safe_load

script_dir = dirname(realpath(__file__))
src_dir = normpath(join(script_dir, "../src"))


warning = """\
################################################################################
#                         AUTOMATICALLY GENERATED FILE                         #
#                                                                              #
#    ! Don't edit this file directly, edit the keywords.yml files in the !     #
#    ! appropriate subdirectories instead.                               !     #
#                                                                              #
################################################################################

"""

def keywords_yaml2txt(file):
    data = safe_load(file)
    txt = ""
    for k, v in data.items():
        if isinstance(k, str) and isinstance(v, list):
            txt += ''.join(map(lambda name: f'{name}\t{k.upper()}\n', v))
            txt += '\n'
    return txt


def main():
    txt = warning
    for root, dirs, files in os.walk(src_dir):
        for f in files:
            if re.match(r'keywords.ya?ml', f):
                with open(join(root, f), 'r+') as yamlfile:
                    name = relpath(root, src_dir)
                    if name == '.': name = 'src'
                    txt += "# " + name + '\n'
                    txt += '#' * (len(name) + 2) + '\n' * 2
                    txt += keywords_yaml2txt(yamlfile)
    with open(join(dirname(script_dir), 'keywords.txt'), 'w') as f:
        f.write(txt)


if __name__ == "__main__":
    main()
