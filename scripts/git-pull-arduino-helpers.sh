#!/usr/bin/env bash

cd "$(dirname "$0")"

git pull arduino-helpers master
git reset ../docs
git reset ../examples/examples.dox
git checkout ../docs
git checkout ../examples/examples.dox
git add ../docs
git add ../examples/examples.dox