#!/bin/bash
# ------------------------------------------------------------------------------
# Travis CI scripts
# Copyright(c) pgRouting Contributors
#
# Install Doxygen
# ------------------------------------------------------------------------------

set -e

if [[ "${DOCUMENTATION}"="OFF" ]]
then
    exit 0
fi  

sudo \
    sudo apt-get --no-install-suggests --no-install-recommends -qq install doxygen  && \
    doxygen --version ;
