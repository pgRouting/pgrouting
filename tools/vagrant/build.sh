#!/bin/bash
# ------------------------------------------------------------------------------
# Vagrant - Virtualized Development 
# Copyright(c) pgRouting Contributors
#
# Build pgRouting
# ------------------------------------------------------------------------------

set -e # Exit script immediately on first error.
#set -x # Print commands and their arguments as they are executed.

cd /vagrant
rm -Rf build 
cmake -H. -Bbuild -DWITH_DD=ON 
cd build 
make 
cd ..

