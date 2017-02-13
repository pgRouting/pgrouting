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
cmake  -DPOSTGRESQL_VERSION=$POSTGRESQL_VERSION -DWITH_DOC=1 -DBUILD_DOXY=1 -DCMAKE_BUILD_TYPE=Debug
make
sudo make install
make doc
make doxy
