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
# Define alias function for psql command
run_psql () {
    PGOPTIONS='--client-min-messages=warning' psql -U $PGUSER  -X -q -v ON_ERROR_STOP=1 --pset pager=off "$@"
    if [ "$?" -ne 0 ]
    then
        echo "Test query failed: $@"
        ERROR=1
    fi
}

run_psql -c "SELECT version()" pgr_test__db__test
run_psql -c "CREATE EXTENSION pgtap" pgr_test__db__test
run_psql -c "SELECT pgtap_version()"
