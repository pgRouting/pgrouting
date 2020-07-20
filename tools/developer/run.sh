#!/bin/bash

set -e


# copy this file into the root of your repository
# adjust to your needs

# This run.sh is intended for 3.x.x
VERSION=$(grep -Po '(?<=project\(PGROUTING VERSION )[^;]+' CMakeLists.txt)
echo "pgRouting VERSION ${VERSION}"

# set up your postgres version and port
PGVERSION="12"
PGPORT="5432"
PGBIN="/usr/lib/postgresql/${PGVERSION}/bin"

# Compiler setup

# When more than one compiler is installed
GCC="8"

# Using the default compiler version
unset GCC


ALLDIRS="
allpairs
alpha_shape
astar
bdAstar
bdDijkstra
bellman_ford
breadthFirstSearch
chinese
common
components
contraction
dagShortestPath
dijkstra
driving_distance
internalQueryTests
ksp
lineGraph
max_flow
mincut
pickDeliver
spanningTree
topologicalSort
topology
transitiveClosure
trsp
tsp
version
vrp_basic
withPoints"

TESTDIRS=${ALLDIRS}
TESTDIRS="dijkstra"


function test_compile {

echo ------------------------------------
echo ------------------------------------
echo "Compiling with G++-$1"
echo ------------------------------------

if [ ! -z "$1" ]; then
    update-alternatives --set gcc "/usr/bin/gcc-$1"
fi


cd build/

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

make -j 4
sudo make install
cd ..


echo
echo --------------------------------------------
echo  Execute documentation queries for a particular directories
echo --------------------------------------------


# choose what is going to be tested while developing
for d in ${TESTDIRS}
do
    #tools/testers/doc_queries_generator.pl  -alg ${d} -documentation  -pgport ${PGPORT}
    tools/testers/doc_queries_generator.pl  -alg ${d} -pgport ${PGPORT}
    tools/developer/taptest.sh  ${d} -p ${PGPORT}
done






echo
echo --------------------------------------------
echo  Update signatures
echo --------------------------------------------

sh tools/release-scripts/get_signatures.sh -p ${PGPORT}


if [[ $(git status | grep 'pgrouting--') ]]; then
    echo "**************************************************"
    echo "           WARNING"
    echo "the signatures changed, copyed the generated files"
    echo "Plese verify the changes are minimal"
    echo "**************************************************"
    git diff sql/sigs
fi


################################
################################
## checks all the repository
#
#  the rest of the script use PGPORT variable
################################
################################
echo
echo --------------------------------------------
echo  Update / Verify NEWS
echo --------------------------------------------
tools/release-scripts/notes2news.pl
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
tools/testers/doc_queries_generator.pl  -documentation  -pgport $PGPORT

tools/testers/doc_queries_generator.pl -pgport $PGPORT

cd build
#rm -rf doc/*
make doc
#rm -rf doxygen/*
make doxy
cd ..

########################################################
# pgTap test all
########################################################

dropdb --if-exists -p $PGPORT ___pgr___test___
createdb  -p $PGPORT ___pgr___test___
echo $PGPORT
sh ./tools/testers/pg_prove_tests.sh vicky $PGPORT
dropdb  -p $PGPORT ___pgr___test___

#tools/testers/update-tester.sh

}

# Uncomment what you need
for compiler in ${GCC}
do
    if [ ! -z "$1" ]; then
        echo "Fresh build"
        rm -rf build/*
    fi
    test_compile ${compiler}
done
