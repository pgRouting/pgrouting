#!/bin/bash

set -e

CPPVERSION=$1
MINOR=$2
MICRO=$3
FULL_VER="$MINOR.$MICRO"

function test_compile {                                                                                                                                                                                                          

echo ------------------------------------
echo ------------------------------------
echo Compiling with $1
echo ------------------------------------

sudo update-alternatives --set gcc /usr/bin/gcc-$1

cd build/
touch tmp_make.txt
touch tmp_make_err.txt
cmake  -DDOC_USE_BOOTSTRAP=ON -DWITH_DOC=ON -DBUILD_DOXY=ON -DCMAKE_BUILD_TYPE=Release .. >> tmp_make.txt

if [[ "$1" == "4.8" ]]; then
    make doc >> tmp_make.txt 
    echo "  - [x] Build Users documentation"
    make doxy >> tmp_make.txt
    echo "  - [x] Build developers documentation"
fi

make >> tmp_make.txt 2>>tmp_make_err.txt

sudo make install >> tmp_make.txt

cd ..

echo "  - [x] Compilation"

tools/testers/algorithm-tester.pl >> build/tmp_make.txt
echo "  - [x] Documentation tests"


dropdb --if-exists ___pgr___test___
createdb  ___pgr___test___
sh ./tools/testers/pg_prove_tests.sh vicky >> build/tmp_make.txt
dropdb  ___pgr___test___
echo '  - [x] Pgtap tests'
}

sudo rm -f /usr/lib/postgresql/9.3/lib/libpgrouting-$MINOR.so
sudo rm -f /usr/share/postgresql/9.3/extension/pgrouting*$FULL_VER*
rm -rf build/*
test_compile $CPPVERSION

exit 0
