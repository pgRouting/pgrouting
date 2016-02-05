#!/bin/bash
# ------------------------------------------------------------------------------
# Travis CI scripts 
# Copyright(c) pgRouting Contributors
#
# Install pgRouting prerequesits
# ------------------------------------------------------------------------------

POSTGRESQL_VERSION="$1"

if [ "$POSTGRESQL_VERSION" == "9.5" ] then

    sudo apt-get install -y postgresql-9.5
    sudo apt-get install -y postgresql-9.5-postgis
    sudo /etc/init.d/postgresql stop 

fi
