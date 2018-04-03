#!/bin/bash

set -e


# This run.sh is intended for 2.6.1
if [ -z $1 ]; then
    VERSION="2.6.1"
else
    VERSION=$1
fi

# when more than one postgres version is installed on the computer
PGSQL_VER="9.5"
PGPORT=5432

function test_compile {

echo ------------------------------------
echo ------------------------------------
echo Compiling with $1
echo ------------------------------------

#sudo update-alternatives --set gcc /usr/bin/gcc-$1

cd build/

# Using all defaults
#cmake ..

# Options Release RelWithDebInfo MinSizeRel Debug
#cmake  -DCMAKE_BUILD_TYPE=Debug ..

# with documentation
#cmake  -DDOC_USE_BOOTSTRAP=ON -DWITH_DOC=ON -DBUILD_DOXY=ON -DPgRouting_DEBUG=ON -DCMAKE_BUILD_TYPE=Debug ..

# when more than one postgres version is installed on the computer
cmake  -DPOSTGRESQL_BIN=/usr/lib/postgresql/$PGSQL_VER/bin -DDOC_USE_BOOTSTRAP=ON -DWITH_DOC=ON -DBUILD_DOXY=ON  -DBUILD_LATEX=ON  -DCMAKE_BUILD_TYPE=Debug ..

make
sudo make install
cd ..


echo
echo --------------------------------------------
echo  Execute documentation queries for a particular directory
echo --------------------------------------------

# - when one postgres version is installed on the computer
tools/testers/algorithm-tester.pl  -alg withPoints -documentation

# - when more than one postgres version is installed on the computer
tools/testers/algorithm-tester.pl  -alg withPoints -documentation  -pgport $PGPORT


echo
echo --------------------------------------------
echo  Execute pgTap test  particular directory
echo --------------------------------------------

# - when one postgres version is installed on the computer
tools/developer/taptest.sh  withPoints/*

# - when more than one postgres version is installed on the computer
tools/developer/taptest.sh  withPoints/* -p $PGPORT

echo
echo --------------------------------------------
echo  Execute pgTap test  particular file
echo --------------------------------------------

# - when one postgres version is installed on the computer
tools/developer/taptest.sh  withPoints/undirected_equalityDD.sql

# - when more than one postgres version is installed on the computer
tools/developer/taptest.sh  withPoints/undirected_equalityDD.sql  -p $PGPORT



echo
echo --------------------------------------------
echo  Verify with signatures did not change
echo --------------------------------------------

# - when one postgres version is installed on the computer
sh tools/release-scripts/get_signatures.sh $VERSION ____sigs_routing____ sql/sigs

# when more than one postgres version is installed on the computer
sh tools/release-scripts/get_signatures.sh $VERSION ____sigs_routing____ sql/sigs -p $PGPORT

# this is for version <2.6.0
#cp build/sql/pgrouting--*.sql tools/sql-update-scripts
if [[ $(git status | grep 'pgrouting--') ]]; then
    echo "**************************************************"
    echo "           WARNING"
    echo "the signatures changed, copying generated files"
    echo "Plese verify the changes are minimal"
    echo "**************************************************"
    git diff
fi

exit 0

################################
################################
## checks all the repository
#
#  the rest of the script use PGPORT variable
################################
################################
echo
echo --------------------------------------------
echo  Verify NEWS
echo --------------------------------------------
release-scripts/notes2news.pl
if [[ $(git status | grep 'NEWS') ]]; then
    echo "**************************************************"
    echo "           WARNING"
    echo "the signatures changed, copying generated files"
    echo "Plese verify the changes are minimal"
    echo "**************************************************"
    git diff NEWS
fi

########################################################
#  Execute documentation queries for the whole project
########################################################
tools/testers/algorithm-tester.pl  -documentation  -pgport $PGPORT

# update the trsp README.md file
mv doc/queries/trsp_notes_v${VERSION}.queries doc/doc/trsp/README.md
cp test/trsp/trsp_notes_v${VERSION}.result doc/trsp/README.md

if [[ $(git status | grep 'trsp_notes') ]]; then
    echo "**************************************************"
    echo "           WARNING"
    echo "The trsp notes changed"
    echo "Plese verify the changes are OK"
    echo "**************************************************"
    git diff
fi




tools/testers/algorithm-tester.pl -documentation  -pgport $PGPORT
tools/testers/algorithm-tester.pl -pgport $PGPORT

cd build
rm -rf doc/*
make doc
rm -rf doxygen/*
make doxy
cd ..

########################################################
# pgTap testing only a particular directory and on a particular file:
########################################################

dropdb --if-exists -p $PGPORT ___pgr___test___
createdb  -p $PGPORT ___pgr___test___
echo $PGPORT
sh ./tools/testers/pg_prove_tests.sh vicky $PGPORT
dropdb  -p $PGPORT ___pgr___test___

#tools/testers/update-tester.sh

}

# Uncomment what you need
#rm -rf build/*
#test_compile 4.8
#rm -rf build/*
#test_compile 4.9
#rm -rf build/*
test_compile 5
#rm -rf build/*
#test_compile 6
#rm -rf build/*
#test_compile 7
