#!/bin/bash
# /*PGR-GNU*****************************************************************
# File: run.sh
# Copyright (c) 2017 pgRouting developers
# Mail: project@pgrouting.org
# ------
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
# ********************************************************************PGR-GNU*/
set -e

DIR=$(git rev-parse --show-toplevel)
pushd "${DIR}" > /dev/null || exit 1


# copy this file into the root of your repository
# adjust to your needs

# This run.sh is intended for 3.x.x
VERSION=$(grep -Po '(?<=project\(PGROUTING VERSION )[^;]+' CMakeLists.txt)
echo "pgRouting VERSION ${VERSION}"

# set up your postgres version, port and compiler (if more than one)
PGVERSION="15"
PGPORT="5432"
PGBIN="/usr/lib/postgresql/${PGVERSION}/bin"
# When more than one compiler is installed
GCC=""

QUERIES_DIRS=$(ls docqueries -1)
TAP_DIRS=$(ls pgtap -1)

QUERIES_DIRS="dijkstra"
TAP_DIRS="dijkstra"

function set_cmake {
    # Using all defaults
    #cmake ..

    # Options Release RelWithDebInfo MinSizeRel Debug
    #cmake  -DCMAKE_BUILD_TYPE=Debug ..

    # Additional debug information
    #cmake -DPgRouting_DEBUG=ON -DCMAKE_BUILD_TYPE=Debug ..

    # with documentation (like the one the website)
    #cmake  -DDOC_USE_BOOTSTRAP=ON -DWITH_DOC=ON ..

    # with developers documentation
    #cmake  -DWITH_DOC=ON -DBUILD_DOXY=ON ..

    # Building using clang
    #CXX=clang++ CC=clang cmake -DPOSTGRESQL_BIN=${PGBIN} -DCMAKE_BUILD_TYPE=Debug ..

    #cmake  -DPOSTGRESQL_BIN=${PGBIN} -DDOC_USE_BOOTSTRAP=ON -DWITH_DOC=ON -DBUILD_DOXY=ON  -DBUILD_LATEX=ON -DCMAKE_BUILD_TYPE=Debug -DES=ON -DPROJECT_DEBUG=ON ..

    # building languages -DES=ON -DJA=ON -DZH_HANS=ON -DDE=ON -DKO=ON
    #cmake  -DPOSTGRESQL_BIN=${PGBIN}  -DDOC_USE_BOOTSTRAP=ON -DWITH_DOC=ON -DBUILD_DOXY=ON  -DBUILD_LATEX=ON -DES=ON  -DCMAKE_BUILD_TYPE=Debug ..

    # check link in documentation
    #cmake  -DPOSTGRESQL_BIN=${PGBIN} -DDOC_USE_BOOTSTRAP=ON -DWITH_DOC=ON -DES=ON -DLINKCHECK=ON -DCMAKE_BUILD_TYPE=Release ..

    cmake  -DPOSTGRESQL_BIN=${PGBIN} -DDOC_USE_BOOTSTRAP=ON -DWITH_DOC=ON -DBUILD_DOXY=ON  -DBUILD_LATEX=ON  -DCMAKE_BUILD_TYPE=Debug ..
}

function tap_test {
    echo --------------------------------------------
    echo pgTap test all
    echo --------------------------------------------

    dropdb --if-exists -p $PGPORT ___pgr___test___
    createdb  -p $PGPORT ___pgr___test___
    echo $PGPORT
    tools/testers/pg_prove_tests.sh vicky $PGPORT
    dropdb  -p $PGPORT ___pgr___test___
}

function action_tests {
    echo --------------------------------------------
    echo  Update signatures
    echo --------------------------------------------

    tools/release-scripts/get_signatures.sh -p ${PGPORT}
    tools/release-scripts/notes2news.pl
    bash tools/scripts/test_signatures.sh
    bash tools/scripts/test_shell.sh
    bash tools/scripts/test_license.sh
    bash tools/scripts/code_checker.sh
    tools/testers/doc_queries_generator.pl  -documentation  -pgport $PGPORT
}

function set_compiler {
    echo ------------------------------------
    echo ------------------------------------
    echo "Compiling with G++-$1"
    echo ------------------------------------

    if [ -n "$1" ]; then
        update-alternatives --set gcc "/usr/bin/gcc-$1"
    fi
}

function build_doc {
    pushd build > /dev/null || exit 1
    #rm -rf doc/* ; rm -rf locale/*/*/*.mo
    rm -rf doc/*
    make doc
    #example on how to only build spanish html
    #make html-es

    # developers documentation
    #rm -rf doxygen/*
    make doxy
    popd > /dev/null || exit 1
}

function build {
    pushd build > /dev/null || exit 1
    set_cmake
    #make  -j 16 VERBOSE=1
    make -j 16
    #make
    sudo make install
    popd > /dev/null || exit 1

}

function test_compile {

    set_compiler "${GCC}"

    build

    echo --------------------------------------------
    echo  Execute tap_directories
    echo --------------------------------------------
    for d in ${TAP_DIRS}
    do
        time bash taptest.sh  "${d}" "-p ${PGPORT}"
    done

    echo --------------------------------------------
    echo  Execute documentation queries
    echo --------------------------------------------
    for d in ${QUERIES_DIRS}
    do
        #tools/testers/doc_queries_generator.pl  -alg "docqueries/${d}" -documentation  -pgport "${PGPORT}"
        #tools/testers/doc_queries_generator.pl  -alg "docqueries/${d}" -debug1  -pgport "${PGPORT}"
        tools/testers/doc_queries_generator.pl  -alg "docqueries/${d}" -pgport "${PGPORT}"
    done

    build_doc
    #exit 0
    tap_test
    action_tests
}

test_compile
