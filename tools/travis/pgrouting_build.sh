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
cmake -DWITH_DD=ON -DPOSTGRESQL_VERSION=$POSTGRESQL_VERSION
#cmake
make
sudo make install
cd ..

./tools/test-runner.pl -pgver $POSTGRESQL_VERSION -ignorenotice
