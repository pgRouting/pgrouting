#!/bin/bash
# ------------------------------------------------------------------------------
# Travis CI scripts 
# Copyright(c) pgRouting Contributors
#
# testing documentation
# ------------------------------------------------------------------------------

set -e

OS="$(lsb_release -c -s)"

if [[ "$OS" != "trusty" ]] ; then
    exit 0
fi


#testing doxygen only on trusty

make doc
make doxy

