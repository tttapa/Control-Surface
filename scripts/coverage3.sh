#!/usr/bin/env bash

set -ex

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

build_dir=`realpath $DIR/../build`
build_dir_cs=`realpath $DIR/../build/CMakeFiles/Control_Surface.dir`

export LOCATION=/tmp
mkdir -p $LOCATION/html

rm -f "$LOCATION"/*.info
rm -rf "$LOCATION"/html/*

gcov_bin="gcov-8"

cd $build_dir
find . -name '*.gcda' -type f -delete
make -j$((`nproc` * 2))

cd "$build_dir_cs"
lcov \
    --capture --initial --directory $build_dir_cs \
    --output-file $LOCATION/cov_base.info \
    --gcov-tool $gcov_bin

cd $build_dir
make check

cd "$build_dir_cs"
lcov \
    --capture --directory $build_dir_cs \
    --output-file $LOCATION/cov_test.info \
    --gcov-tool $gcov_bin

lcov \
    --add-tracefile $LOCATION/cov_base.info \
    --add-tracefile $LOCATION/cov_test.info \
    --output-file $LOCATION/cov_total.info \
    --gcov-tool $gcov_bin

lcov \
    --remove $LOCATION/cov_total.info \
    '/usr/include/*' '/usr/lib/*' \
    '*/ArduinoMock/*' '*/mock/*' \
    '*/googletest/*' \
    '*/test/*' \
    --output-file $LOCATION/cov_filtered.info \
    --gcov-tool $gcov_bin

genhtml \
    --prefix `realpath "$DIR/.."` \
    $LOCATION/cov_filtered.info \
    --output-directory=$LOCATION/html \
    --legend --title `cd $DIR/.. && git rev-parse HEAD`
