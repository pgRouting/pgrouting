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
cmake  -DPOSTGRESQL_VERSION=$POSTGRESQL_VERSION -DWITH_DOC -DWITH_DOXY
make
sudo make install

