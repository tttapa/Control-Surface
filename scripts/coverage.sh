#!/usr/bin/env bash

set -ex

dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

proj_dir=`realpath "$dir"/..`
build_dir=`realpath "$proj_dir"/build`

dest="$proj_dir"/doc/Coverage
mkdir -p "$dest"/html

rm -f "$dest"/*.info
rm -rf "$dest"/html/*

# gcov_bin="gcov-9"
gcov_bin="$dir/llvm-cov-gcov.sh"
branches=0

cd "$proj_dir"
pwd

lcov \
    --zerocounters \
    --directory .

make -C "$build_dir" -j$((`nproc` * 2))

lcov \
    --capture --initial --directory . \
    --output-file "$dest"/coverage_base.info \
    --gcov-tool "$gcov_bin" \
    --rc lcov_branch_coverage=$branches

make -C "$build_dir" check

lcov \
    --capture --directory . \
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
