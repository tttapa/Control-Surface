#!/usr/bin/env bash

set -ex

dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

proj_dir=`realpath "$dir"/..`
build_dir=`realpath "$proj_dir"/build`

dest=/tmp
mkdir -p "$dest"/html

rm -f "$dest"/*.info
rm -rf "$dest"/html/*

gcov_bin="gcov-8"

cd "$proj_dir"
pwd

lcov \
    --zerocounters \
    --directory .

make -C "$build_dir" -j$((`nproc` * 2))

lcov \
    --capture --initial --directory . \
    --output-file "$dest"/cov_base.info \
    --gcov-tool $gcov_bin

make -C "$build_dir" check

lcov \
    --capture --directory . \
    --output-file "$dest"/cov_test.info \
    --gcov-tool $gcov_bin

lcov \
    --add-tracefile "$dest"/cov_base.info \
    --add-tracefile "$dest"/cov_test.info \
    --output-file "$dest"/cov_total.info \
    --gcov-tool $gcov_bin

lcov \
    --remove "$dest"/cov_total.info \
    '/usr/include/*' '/usr/lib/*' \
    '*/ArduinoMock/*' '*/mock/*' \
    '*/googletest/*' \
    '*/test/*' \
    --output-file "$dest"/cov_filtered.info \
    --gcov-tool $gcov_bin

genhtml \
    --prefix `realpath "$proj_dir"` \
    "$dest"/cov_filtered.info \
    --output-directory="$dest"/html \
    --legend --title `cd "$proj_dir" && git rev-parse HEAD`
