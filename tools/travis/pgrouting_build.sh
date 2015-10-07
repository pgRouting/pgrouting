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
#cmake -DWITH_DD=ON 
mkdir build
cd build
cmake -DCMAKE_CXX_COMPILER=g++-4.8 \
      -DCMAKE_C_COMPILER=gcc-4.8 ..
make
sudo make install
