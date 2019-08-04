#!/bin/bash

# Copies the generated Doxygen and lcov HTML output to tttapa.github.io, then 
# commits these changes to the tttapa.github.io repository, and pushes them to 
# GitHub.

cd "$(dirname "$0")"

commit=$(git rev-parse HEAD)
rm -r ~/GitHub/tttapa.github.io/Control-Surface/Doc/Doxygen 
mkdir ~/GitHub/tttapa.github.io/Control-Surface/Doc/Doxygen 
cp -a ../html/. ~/GitHub/tttapa.github.io/Control-Surface/Doc/Doxygen
rm -r ~/GitHub/tttapa.github.io/Control-Surface/Doc/Coverage 
mkdir ~/GitHub/tttapa.github.io/Control-Surface/Doc/Coverage 
cp -a ../../Coverage/html/. ~/GitHub/tttapa.github.io/Control-Surface/Doc/Coverage
cd ~/GitHub/tttapa.github.io
git add ./Control-Surface/Doc
git commit -m "Update Control Surface documentation to $commit"
git push origin master