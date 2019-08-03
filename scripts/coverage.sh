#!/usr/bin/env bash

set -ex

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

build_dir=`realpath $DIR/../build`

export LOCATION=/tmp
mkdir -p $LOCATION/cov

copy_cov_file() {
    local dst_name=`echo $0 | sed 's/\.\///g; s/\//-/g'`
    cp $0 "$LOCATION/cov/$dst_name"
}
export -f copy_cov_file

rm -f $LOCATION/cov_base.info $LOCATION/cov_test.info $LOCATION/cov_filtered.info $LOCATION/cov_total.info
rm -rf "$LOCATION/cov/*"

cd "$DIR/../build"
find . \
    \( -name '*.gcno' -o -name '*.gcda' \) -type f \
    -delete

cd $build_dir
make -j$((`nproc` * 2))

cd "$DIR/../build/CMakeFiles/Control_Surface.dir/src"
find . \
    \( -name '*.gcno' -o -name '*.gcda' \) -type f \
    -exec bash -c 'copy_cov_file "$0"' {} \;
cd $LOCATION/cov
gcov *

lcov \
    --no-external --capture --initial --directory $LOCATION/cov \
    --output-file $LOCATION/cov_base.info

cd $build_dir
make test

cd "$DIR/../build/CMakeFiles/Control_Surface.dir/src"
find . \
    \( -name '*.gcno' -o -name '*.gcda' \) -type f \
    -exec bash -c 'copy_cov_file "$0"' {} \;
cd $LOCATION/cov
gcov *

lcov \
    --no-external --capture --directory $LOCATION/cov \
    --output-file $LOCATION/cov_test.info

lcov \
    --add-tracefile $LOCATION/cov_base.info \
    --add-tracefile $LOCATION/cov_test.info \
    --output-file $LOCATION/cov_total.info

lcov \
    --remove $LOCATION/cov_total.info \
    '/usr/include/*' '/usr/lib/*' \
    -o $LOCATION/cov_filtered.info

rm -rf $LOCATION/html
mkdir $LOCATION/html

genhtml \
    --prefix `realpath "$DIR/.."` \
    source $LOCATION/cov_filtered.info \
    --legend --title `git rev-parse HEAD` \
    --output-directory=$LOCATION/html
