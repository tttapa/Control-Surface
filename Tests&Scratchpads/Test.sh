#!/bin/bash
cd $( dirname "${BASH_SOURCE[0]}" )/../src/
tests=($(find -type d -name Tests))
for test in "${tests[@]}"
do
    # test=$( dirname "$test")
    echo "Building and running tests from $test"
    make -C "$test"
done