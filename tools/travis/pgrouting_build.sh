#!/bin/bash
# ------------------------------------------------------------------------------
# Travis CI scripts
# Copyright(c) pgRouting Contributors
# ------------------------------------------------------------------------------

cmake -DWITH_DD=ON 
make
sudo make install
