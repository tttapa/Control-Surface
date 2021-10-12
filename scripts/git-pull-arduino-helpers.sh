#!/usr/bin/env bash

cd "$(dirname "$0")"

read -p "This script will reset and clean 'docs'. Press enter to confirm. "

git pull arduino-helpers master
git reset ../docs
git reset ../examples/examples.dox
git checkout ../docs
git clean -d ../docs -f
git checkout ../examples/examples.dox
git add ../docs
git add ../examples/examples.dox