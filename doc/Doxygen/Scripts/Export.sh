#!/bin/bash

# Copies the generated Doxygen HTML output to tttapa.github.io, then commits
# these changes to the tttapa.github.io repository, and pushes them to GitHub.

cd "$(dirname "$0")"

commit=$(git rev-parse HEAD)
rm -r ~/GitHub/tttapa.github.io/Control-Surface/Doc/Doxygen 
mkdir ~/GitHub/tttapa.github.io/Control-Surface/Doc/Doxygen 
cp -a ../html/. ~/GitHub/tttapa.github.io/Control-Surface/Doc/Doxygen
cd ~/GitHub/tttapa.github.io
git add ./Control-Surface/Doc/Doxygen
git commit -m "Update Control Surface documentation to $commit"
git push origin master