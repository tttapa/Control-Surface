#!/usr/bin/env bash

set -ex

dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

proj_dir=`realpath "$dir"/..`
build_dir=`pwd`

html_dest="$proj_dir/docs/Coverage"
dest="$build_dir/coverage"
mkdir -p "$dest"
mkdir -p "$html_dest"

rm -f "$dest/*.info"
rm -rf "$html_dest/*"

if [ "${1,,}" == "clang" ]; then
    gcov_bin="$dir/llvm-cov-gcov.sh"
    # Replace the default c++filt program with LLVM/Clang's version
    mkdir -p /tmp/clang-cxxfilt
    ln -sf `which llvm-cxxfilt` /tmp/clang-cxxfilt/c++filt
    export PATH=/tmp/clang-cxxfilt:$PATH
    which c++filt
else
    gcov_bin="gcov"
fi

branches=0

cd "$proj_dir"
pwd

lcov \
    --zerocounters \
    --directory "$build_dir"

cmake -S "$proj_dir" -B "$build_dir"
cmake --build "$build_dir" -j$(nproc)

lcov \
    --capture --initial --directory "$build_dir" \
    --output-file "$dest"/coverage_base.info \
    --gcov-tool "$gcov_bin" \
    --rc lcov_branch_coverage=$branches

cmake --build "$build_dir" -j$(nproc) --target check

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
    '*/gtest/*' '*/gmock/*' \
    '*/test/*' \
    --output-file "$dest"/coverage_filtered.info \
    --gcov-tool "$gcov_bin" \
    --rc lcov_branch_coverage=$branches

genhtml \
    --prefix `realpath "$proj_dir"` \
    "$dest"/coverage_filtered.info \
    --output-directory="$html_dest" \
    --legend --title `cd "$proj_dir" && git rev-parse HEAD` \
    --rc lcov_branch_coverage=$branches \
    -s \
    --demangle-cpp

./scripts/coverage-badge.py