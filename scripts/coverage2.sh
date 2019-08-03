#!/usr/bin/env bash

set -ex

export gcov_bin="gcov-8"
export dest=/tmp

dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
build_dir=$(realpath $dir/../build)

mkdir -p "$dest"/coverage
mkdir -p "$dest"/html

rm -f "$dest"/*.info
rm -rf "$dest"/cov/*
rm -rf "$dest"/html/*


copy_cov_file() {
    local dst_name=`echo $0 | sed 's/\.\///g; s/\//-/g'`
    cp $0 "$dest/coverage/$dst_name"
}
export -f copy_cov_file

cd "$build_dir"
find . -name '*.gcda' -type f -delete
make -j$((`nproc` * 2))

find . \
    \( -name '*.gcno' \) -type f \
    -exec bash -c 'copy_cov_file "$0"' {} \;

cd "$dest"/cov
# "$gcov_bin" *.o
lcov \
    --capture --initial --directory "$dest"/coverage \
    --output-file "$dest"/coverage_base.info \
    --gcov-tool $gcov_bin

cd "$build_dir"
make check

find . \
    \( -name '*.gcda' \) -type f \
    -exec bash -c 'copy_cov_file "$0"' {} \;

cd "$dest"/cov
# "$gcov_bin" *.o
lcov \
    --capture --directory "$dest"/coverage \
    --output-file "$dest"/coverage_test.info \
    --gcov-tool $gcov_bin

lcov \
    --add-tracefile "$dest"/coverage_base.info \
    --add-tracefile "$dest"/coverage_test.info \
    --output-file "$dest"/coverage_total.info \
    --gcov-tool $gcov_bin

lcov \
    --remove "$dest"/coverage_total.info \
    '/usr/include/*' '/usr/lib/*' \
    '*/ArduinoMock/*' '*/mock/*' \
    '*/googletest/*' \
    '*/test/*' \
    --output-file "$dest"/coverage_filtered.info \
    --gcov-tool $gcov_bin

genhtml \
    --prefix `realpath "$dir/.."` \
    "$dest"/coverage_filtered.info \
    --output-directory=$dest/html \
    --legend --title `cd $dir/.. && git rev-parse HEAD`
