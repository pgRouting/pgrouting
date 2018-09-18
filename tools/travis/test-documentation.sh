#!/bin/bash
# ------------------------------------------------------------------------------
# Travis CI scripts
# Copyright(c) pgRouting Contributors
#
# testing documentation
# ------------------------------------------------------------------------------

set -e

#testing doxygen only on trusty

echo "generating users documentation"
make doc
echo "generating developers documentation"
make doxy

