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

# IMPORTANT
# Copy this file into the root of your repository
# Adjust to your developing needs
#  - Some example of adjustments are mentioned

# This run.sh is intended for 4.x.x
VERSION=$(grep -Po '(?<=project\(PGROUTING VERSION )[^;]+' CMakeLists.txt)

# EXAMPLE: For macOS Homebrew instead of the above use with ggrep as follows
# VERSION=$(ggrep -Po '(?<=project\(PGROUTING VERSION )[^;]+' CMakeLists.txt)

echo "pgRouting VERSION ${VERSION}"

# set up your postgres version, port and compiler (if more than one)
# EXAMPLE: For macOS Homebrew this line is not needed
PGVERSION="15"

PGBIN="/usr/lib/postgresql/${PGVERSION}/bin"

# EXAMPLE: For macOS Homebrew it has a different path
# PGBIN="/opt/homebrew/bin"

PGPORT="5432"

# When more than one compiler is installed
GCC=""

# -------------- documentation queries and tap tests ---------------

# To process all subdirectories
QUERIES_DIRS=$(ls -1 docqueries)
TAP_DIRS=$(ls -1 pgtap)

# To process a particular subdirectory
QUERIES_DIRS="dijkstra"
TAP_DIRS="dijkstra"

# To skip processing queries and tap tests
#QUERIES_DIRS=""
#TAP_DIRS=""

function set_cmake {
    # inspect options
    # cmake -LH ..

    # Building using clang
    #CXX=clang++ CC=clang cmake -DPOSTGRESQL_BIN=${PGBIN} ..

    # Building using default compiler (g++)
    #cmake -DPOSTGRESQL_BIN=${PGBIN} ..

    # Options Release RelWithDebInfo MinSizeRel Debug
    #cmake -DCMAKE_BUILD_TYPE=Debug ..

    # Build documentation with booststrap
    #cmake -DDOC_USE_BOOTSTRAP=ON ..

    # Build only english documentation
    #cmake -DES=OFF -DZH_HANS=OFF ..

    # sphinx flags
    #cmake -DSPHINX_FLAGS='-E -W' ..

    # Dont build documentation
    #cmake -DBUILD_HTML=OFF ..

    # Additional debug information
    #cmake -DPROJECT_DEBUG=ON ..

    # For some checks
    #cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..

    # build developers documentation
    #cmake -DBUILD_DOXY=ON ..

    # build all languages (all others are on by default)
    #cmake -DDOC_USE_BOOTSTRAP=ON -DJA=ON -DDE=ON -DKO=ON ..

    # build one language for checking warnings, for example swedish
    cmake -DDOC_USE_BOOTSTRAP=ON -DBUILD_HTML=ON -DEN=ON -DES=OFF -DSV=ON -DZH_HANS=OFF -DSPHINX_FLAGS='-W -w sphinx-errs.txt -v -n -j auto --keep-going' ..

    # check link in documentation
    #cmake -DBUILD_LINKCHECK=ON ..

    # build locale
    #cmake -DBUILD_LOCALE=ON ..

    # Put all options into one convenient line
    cmake -DPOSTGRESQL_BIN=${PGBIN} -DCMAKE_BUILD_TYPE=Debug -DBUILD_HTML=ON -DES=OFF -DZH_HANS=OFF -DSV=OFF -DDOC_USE_BOOTSTRAP=ON ..

    # Working with code only
    cmake -DPOSTGRESQL_BIN=${PGBIN} -DCMAKE_BUILD_TYPE=Debug -DBUILD_HTML=OFF ..

    # Use clang tidy
    #cmake -DUSE_CLANG_TIDY=ON ..
}

function tap_test {
    echo --------------------------------------------
    echo pgTap test all
    echo --------------------------------------------

    dropdb --if-exists -p $PGPORT ___pgr___test___
    createdb  -p $PGPORT ___pgr___test___
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
    # Start from scratch
    #rm -rf locale/*/*/*.mo
    # Clean only generated files while preserving custom content
    #find doc -type f \( -name "*.html" -o -name "*.pdf" \) -delete
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
    sudo make install

    # EXAMPLE: For macOS Homebrew
    # make install

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
        # Show debug messages from pgrouting
        #tools/testers/doc_queries_generator.pl  -alg "docqueries/${d}" -level DEBUG3  -pgport "${PGPORT}"
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
