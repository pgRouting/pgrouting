#!/bin/bash

set -e

echo "$1"

if [ -z "$1" ]; then
    COMPILER="8"
    PGSQL_VER="12"
else
    COMPILER="4.8 4.9 5 6 7 8"
    PGSQL_VER="12"
fi

echo "$COMPILER"
echo "$PGSQL_VER"


function test_compile {

echo ------------------------------------
echo ------------------------------------
echo Compiling with "$1"
echo ------------------------------------

sudo update-alternatives --set gcc "/usr/bin/gcc-$1"
cd build/
echo "../tmp_make_$1_$2_$3.txt"
cmake  "-DPOSTGRESQL_BIN=/usr/lib/postgresql/$2/bin" "-DCMAKE_BUILD_TYPE=$3" .. >> "../tmp_make_$1_$2_$3.txt"
make -j 4 >> "../tmp_make_$1_$2_$3.txt"
sudo make install
cd ..
}

for v in $COMPILER
do
    for p in $PGSQL_VER
    do
        #rm -rf build/*
        test_compile "$v" "$p" Debug
        echo "$v on $p Debug completed"
        #rm -rf build/*
        #test_compile $v $p Release
        #echo "$v on $p Release completed"
    done
done
