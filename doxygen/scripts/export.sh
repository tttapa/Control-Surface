#!/bin/bash

# Copies the generated Doxygen and lcov HTML output to tttapa.github.io, then 
# commits these changes to the tttapa.github.io repository, and pushes them to 
# GitHub.

cd "$(dirname "$0")"

commit=$(git rev-parse HEAD)
rm -r ~/GitHub/tttapa.github.io/Control-Surface/Doc/Doxygen/html
mkdir ~/GitHub/tttapa.github.io/Control-Surface/Doc/Doxygen/html
cp -a ../../docs/Doxygen/* ~/GitHub/tttapa.github.io/Control-Surface/Doc/Doxygen/html
rm -r ~/GitHub/tttapa.github.io/Control-Surface/Doc/Coverage/html
mkdir ~/GitHub/tttapa.github.io/Control-Surface/Doc/Coverage/html
cp -a ../../docs/Coverage/* ~/GitHub/tttapa.github.io/Control-Surface/Doc/Coverage/html
cd ~/GitHub/tttapa.github.io
git add ./Control-Surface/Doc
git commit -m "Update Control Surface documentation to $commit"
git push origin master