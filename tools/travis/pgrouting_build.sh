#!/bin/bash
# ------------------------------------------------------------------------------
# Travis CI scripts 
# Copyright(c) pgRouting Contributors
#
# Build pgRouting
# ------------------------------------------------------------------------------

# exit script on error
set -e 

# build pgRouting


mkdir build
cd build
cmake -DCMAKE_CXX_COMPILER=g++-4.8 \
      -DCMAKE_C_COMPILER=gcc-4.8 \
      -DPOSTGRESQL_VERSION=${POSTGRESQL_VERSION} \
      -DPOSTGIS_VERSION=${POSTGIS_VERSION} \
      ..
make
sudo make install
cd ..

./tools/test-runner.pl -pgver $POSTGRESQL_VERSION -ignorenotice
