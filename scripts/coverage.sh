#!/usr/bin/env bash

set -ex

dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

proj_dir=`realpath "$dir"/..`
build_dir=`pwd`

dest="$proj_dir"/doc/Coverage
mkdir -p "$dest"/html

rm -f "$dest"/*.info
rm -rf "$dest"/html/*

if [ "${1,,}" == "clang" ]; then
    gcov_bin="$dir/llvm-cov-gcov.sh"
else
    gcov_bin="gcov"
fi

branches=0

cd "$proj_dir"
pwd

lcov \
    --zerocounters \
    --directory "$build_dir"

make -C "$build_dir" -j$((`nproc` * 2))

lcov \
    --capture --initial --directory "$build_dir" \
    --output-file "$dest"/coverage_base.info \
    --gcov-tool "$gcov_bin" \
    --rc lcov_branch_coverage=$branches

make -C "$build_dir" check

lcov \
    --capture --directory "$build_dir" \
    --output-file "$dest"/coverage_test.info \
    --gcov-tool "$gcov_bin" \
    --rc lcov_branch_coverage=$branches

lcov \
    --add-tracefile "$dest"/coverage_base.info \
    --add-tracefile "$dest"/coverage_test.info \
    --output-file "$dest"/coverage_total.info \
    --gcov-tool "$gcov_bin" \
    --rc lcov_branch_coverage=$branches

lcov \
    --remove "$dest"/coverage_total.info \
    '/usr/include/*' '/usr/lib/*' \
    '*/ArduinoMock/*' '*/mock/*' \
    '*/googletest/*' \
    '*/test/*' \
    --output-file "$dest"/coverage_filtered.info \
    --gcov-tool "$gcov_bin" \
    --rc lcov_branch_coverage=$branches

genhtml \
    --prefix `realpath "$proj_dir"` \
    "$dest"/coverage_filtered.info \
    --output-directory="$dest"/html \
    --legend --title `cd "$proj_dir" && git rev-parse HEAD` \
    --rc lcov_branch_coverage=$branches
