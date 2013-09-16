#!/bin/bash
# ------------------------------------------------------------------------------
# Vagrant - Virtualized Development 
# Copyright(c) pgRouting Contributors
#
# Install pgRouting
# ------------------------------------------------------------------------------

set -e # Exit script immediately on first error.
#set -x # Print commands and their arguments as they are executed.

sudo dpkg -r pgrouting 
cd build 
sudo checkinstall -y --nodoc --pkgversion=2.0.0 --pkgname=pgrouting 
cd ..

