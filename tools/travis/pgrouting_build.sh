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
cmake  -DPOSTGRESQL_VERSION=$POSTGRESQL_VERSION -DHTML=ON -DBUILD_DOXY=ON -DCMAKE_BUILD_TYPE=Debug ..
make
sudo make install
make doc
make doxy
