#!/usr/bin/env bash
cd "$(dirname "$0")"

### Functions for parallel execution with FIFO

open_sem(){
    mkfifo pipe-$$
    exec 3<>pipe-$$
    rm pipe-$$
    local i=$1
    for((;i>0;i--)); do
        printf %s 000 >&3
    done
}

failed=0

build_with_lock(){
    local name=$(basename -- "$file")
    local result
    read -u 3 -n 3 result # && ((0==result)) || return $result
    echo -e "\e[1;94mBuilding Example: $name\e[0m"
    if [ $result -ne 0 ]; then failed=$[$failed + 1]; fi
    (
        ### Setup for arduino-builder
        arduino=$HOME/opt/arduino-1.8.9
        version=10809
        libraries=$HOME/Arduino/libraries

        if grep -q "ESP32 only" "$file"; then
            board=esp32:esp32:esp32thing:FlashFreq=80,PartitionScheme=default,UploadSpeed=921600,DebugLevel=none
        else
            board=teensy:avr:teensy31:speed=96,usb=serialmidiaudio,opt=o2std,keys=en-us
        fi

        arduino-builder \
            -hardware $arduino/hardware -hardware $HOME/.arduino15/packages \
            -tools $arduino/tools-builder -tools $HOME/.arduino15/packages \
            -libraries $libraries \
            -fqbn $board \
            -core-api-version $version \
            -warnings all \
            -compile $file;
        res=$?
        if [ $res == 0 ]
        then
            echo -e "\e[1;32mBuilt $name successfully! ✔\e[0m\n"
        else
            echo -e "\e[1;31mBuilding $name failed!\e[0m\n"
            realpath $1
        fi
        printf '%.3d' $res >&3
    )&
}

get_failures(){
    local i=$1
    for((;i>0;i--)); do
        local result
    read -u 3 -n 3 result
    if [ $result -ne 0 ]
    then
       failed=$[$failed + 1]
    fi
    done
    return $failed
}

### First argument is the number of concurrent build processes
N=1
if [ "$#" -ge 2 ]; then
    echo "Illegal number of parameters"
    exit -1
elif [ "$#" -eq 1 ]; then
    N=$1
fi
echo Building $N examples concurrently

### Find files to compile
test_files=($(find ../test -name '*.ino'))
example_files=($(find ../examples -name '*.ino'))
files=("${test_files[@]}" "${example_files[@]}")

### Start the FIFO and build all examples
open_sem $N

for file in "${files[@]}"
do 
    build_with_lock $file
done

wait
get_failures $N
if [ $failed -eq 0 ]
then
    echo -e "\e[1;32m=========================================\e[0m"
    echo -e "\e[1;32m    All examples built successfully ✔    \e[0m"
    echo -e "\e[1;32m=========================================\e[0m"
else
    echo -e "\e[1;31m=========================================\e[0m"
    echo -e "\e[1;31m      $failed example(s) failed to build \e[0m"
    echo -e "\e[1;31m=========================================\e[0m"
fi
exit $failed