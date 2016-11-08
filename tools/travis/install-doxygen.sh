#!/bin/bash
# ------------------------------------------------------------------------------
# Travis CI scripts 
# Copyright(c) pgRouting Contributors
#
# Install Doxygen
# ------------------------------------------------------------------------------

set -e

OS="$(lsb_release -c -s)"

if [[ "$OS" != "trusty" ]] ; then
    exit 0
fi


#install doxygen only on trusty

sudo \
    sudo apt-get --no-install-suggests --no-install-recommends -qq install doxygen  && \
    doxygen --version ;

#sudo add-apt-repository ppa:libreoffice/libreoffice-4-4 -y && \
#    sudo apt-get -qq update && \
#    sudo apt-get --no-install-suggests --no-install-recommends -qq install doxygen  && \
#    doxygen --version ;

