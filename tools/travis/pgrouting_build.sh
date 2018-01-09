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
cmake  -DPOSTGRESQL_VERSION=$POSTGRESQL_VERSION -DWITH_DOC=ON -DCMAKE_BUILD_TYPE=Debug ..

if [[ "$DOCUMENTATION" == "ON" ]]
then
    make doc
else
    make
    sudo make install
fi
