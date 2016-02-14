#!/bin/bash
# ------------------------------------------------------------------------------
# Travis CI scripts 
# Copyright(c) pgRouting Contributors
#
# Install Pgtap
# ------------------------------------------------------------------------------

POSTGRESQL_VERSION="$1"

# exit script on error
set -e 

echo "Installing pgtap ... this may take some time."
PGUSER="postgres"

sudo cp /usr/lib/postgresql/$1/bin/pg_config /usr/bin/pg_config

wget https://github.com/theory/pgtap/archive/master.zip
unzip master.zip
cd pgtap-master
make
#make installcheck
sudo make install

#sh test_pgtap.sh
