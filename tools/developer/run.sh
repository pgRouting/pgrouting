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

    # using a particular PostgreSQL configuration
    #cmake -DPOSTGRESQL_PG_CONFIG="/usr/lib/postgresql/${PGVERSION}/bin/pg_config" ..

    # Building using clang
    #CXX=clang++ CC=clang cmake -DPOSTGRESQL_BIN=${PGBIN} -DCMAKE_BUILD_TYPE=Debug  -DDOC_USE_BOOTSTRAP=ON -DWITH_DOC=ON -DBUILD_DOXY=OFF ..

    # Building with debug on
    #cmake  -DPOSTGRESQL_BIN=${PGBIN} -DDOC_USE_BOOTSTRAP=ON -DWITH_DOC=ON -DBUILD_DOXY=ON  -DBUILD_LATEX=ON -DCMAKE_BUILD_TYPE=Debug -DES=ON -DPROJECT_DEBUG=ON ..

    # building languages -DES=ON -DJA=ON -DZH_HANS=ON -DDE=ON -DKO=ON and CMAKE_EXPORT_COMPILE_COMMANDS for static analysis tools.
    #cmake  -DPOSTGRESQL_BIN=${PGBIN} -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DDOC_USE_BOOTSTRAP=ON -DWITH_DOC=ON -DBUILD_DOXY=ON  -DBUILD_LATEX=ON -DES=ON  -DCMAKE_BUILD_TYPE=Debug ..

    # check link in documentation
    #cmake  -DPOSTGRESQL_BIN=${PGBIN} -DDOC_USE_BOOTSTRAP=ON -DWITH_DOC=ON -DES=ON -DLINKCHECK=ON -DCMAKE_BUILD_TYPE=Release ..

    # build only english
    cmake  -DPOSTGRESQL_BIN=${PGBIN} -DDOC_USE_BOOTSTRAP=ON -DWITH_DOC=ON -DBUILD_DOXY=ON  -DBUILD_LATEX=ON  -DCMAKE_BUILD_TYPE=Debug ..
}

function tap_test {
    echo --------------------------------------------
    echo pgTap test all
    echo --------------------------------------------

    dropdb --if-exists -p $PGPORT ___pgr___test___
    createdb  -p $PGPORT ___pgr___test___
    echo $PGPORT
    tools/testers/pg_prove_tests.sh "$PGUSER" $PGPORT
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
        export CC="/usr/bin/gcc-$1"
        export CXX="/usr/bin/g++-$1"
    fi
}

function build_doc {
    pushd build > /dev/null || exit 1
    #rm -rf doc/* ; rm -rf locale/*/*/*.mo
    # Clean only generated files while preserving custom content
    find doc -type f \( -name "*.html" -o -name "*.pdf" \) -delete
    make doc
    #example on how to only build spanish html
    #make html-es

    # developers documentation
    #rm -rf doxygen/*
    make doxy
    popd > /dev/null || exit 1
}

function check {
    pushd build > /dev/null || exit 1
    cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..

    # Run with error handling and report generation
    cppcheck --project=compile_commands.json \
        --enable=all \
        --suppress=missingIncludeSystem \
        --error-exitcode=1 \
        --output-file=cppcheck-report.txt 2>&1 || {
        echo "Static analysis failed. See build/cppcheck-report.txt for details"
        return 1
    }
    popd > /dev/null || exit 1
}

function tidy_with_clang {
    local base_branch=${1:-"upstream/develop"}
    .github/scripts/tidy-vs-commit.sh "$base_branch" || {
        echo "clang-tidy checks failed"
        return 1
    }
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
    echo  Execute documentation queries
    echo --------------------------------------------
    for d in ${QUERIES_DIRS}
    do
        # generate the documentation queries
        #tools/testers/doc_queries_generator.pl  -alg "docqueries/${d}" -documentation  -pgport "${PGPORT}"
        # Show warnings
        #tools/testers/doc_queries_generator.pl  -alg "docqueries/${d}" -level WARNING  -pgport "${PGPORT}"
        # Compare differences on results
        tools/testers/doc_queries_generator.pl  -alg "docqueries/${d}" -pgport "${PGPORT}"
    done

    echo --------------------------------------------
    echo  Execute tap_directories
    echo --------------------------------------------
    for d in ${TAP_DIRS}
    do
        time bash taptest.sh  "${d}" "-p ${PGPORT}"
    done

    tap_test
    tools/testers/doc_queries_generator.pl -pgport $PGPORT

    build_doc
    tap_test
    action_tests
}

test_compile
