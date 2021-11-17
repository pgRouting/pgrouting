#!/bin/bash

set -e

DIR=$(git rev-parse --show-toplevel)
pushd "${DIR}" > /dev/null || exit 1


# copy this file into the root of your repository
# adjust to your needs

# This run.sh is intended for 3.x.x
VERSION=$(grep -Po '(?<=project\(PGROUTING VERSION )[^;]+' CMakeLists.txt)
echo "pgRouting VERSION ${VERSION}"

# set up your postgres version, port and compiler (if more than one)
PGVERSION="13"
PGPORT="5432"
PGBIN="/usr/lib/postgresql/${PGVERSION}/bin"
# When more than one compiler is installed
GCC=""

QUERIES_DIRS=$(ls docqueries -1)
TAP_DIRS=$(ls pgtap -1)

QUERIES_DIRS="dijkstra"
TAP_DIRS=""

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
    #rm -rf doc/*
    make doc
    #rm -rf doxygen/*
    make doxy
    popd > /dev/null || exit 1
}


function test_compile {

    set_compiler ${GCC}


    pushd build > /dev/null || exit 1
    set_cmake
    make -j 4
    sudo make install
    popd > /dev/null || exit 1


    echo --------------------------------------------
    echo  Execute tap_directories
    echo --------------------------------------------
    for d in ${TAP_DIRS}
    do
        bash taptest.sh  "${d}" "-p ${PGPORT}"
    done

    echo --------------------------------------------
    echo  Execute documentation queries
    echo --------------------------------------------
    for d in ${QUERIES_DIRS}
    do
        #tools/testers/doc_queries_generator.pl  -alg "${d}" -documentation  -pgport "${PGPORT}"
        tools/testers/doc_queries_generator.pl  -alg "${d}" -pgport "${PGPORT}"
    done

    tap_test
    action_tests
}

test_compile
