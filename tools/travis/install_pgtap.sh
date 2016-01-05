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

wget https://github.com/theory/pgtap/archive/master.zip
unzip master.zip
cd pgtap-master
make
make installcheck
sudo make install
cd ..
createdb pgr_test__db__test
psql -c "SELECT version()" pgr_test__db__test
psql -c "CREATE EXTENSION pgtap" pgr_test__db__test
psql -c "SELECT pgtap_version()"
