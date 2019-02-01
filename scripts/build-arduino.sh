#!/usr/bin/env bash
cd "$(dirname "$0")"

board=teensy:avr:teensy31:speed=96,usb=serialmidiaudio,opt=o2std,keys=en-us
arduino=$HOME/opt/arduino-1.8.5
version=10805
libraries=$HOME/Arduino/libraries

test_files=($(find ../test -name '*.ino'))
example_files=($(find ../examples -name '*.ino'))
files=("${test_files[@]}" "${example_files[@]}")

for file in "${files[@]}"
do 
    name=$(basename -- "$file")
    echo -e "\e[1;94mCompiling Example: $name\e[0m"
    arduino-builder \
        -hardware $arduino/hardware/ \
        -tools $arduino/tools-builder \
        -libraries $libraries \
        -fqbn $board \
        -core-api-version $version \
        -warnings all \
        -compile $file

    if [ $? -eq 0 ]; then
        echo -e "\e[1;32mDone ✔\e[0m\n"
    else
        result=$?
        echo -e "\e[1;31mFailed!\e[0m\n"
        exit $result
    fi
done
echo -e "\e[1;32m=======================================\e[0m"
echo -e "\e[1;32m   All examples built successfully ✔   \e[0m"
echo -e "\e[1;32m=======================================\e[0m"