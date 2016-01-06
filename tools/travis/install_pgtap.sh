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

sudo cp /usr/lib/postgresql/$1/bin/pg_config /usr/bin/pg_config

wget https://github.com/theory/pgtap/archive/master.zip
unzip master.zip
cd pgtap-master
make
make installcheck

#sudo make install

#installing manually
sudo cp pgtap.control /usr/share/postgresql/$1/extension/pgtap.control
sudo cp sql/pgtap--0.95.0.sql /usr/share/postgresql/$1/extension/pgtap--0.95.0.sql
sudo cp sql/pgtap-core--0.95.0.sql  /usr/share/postgresql/$1/extension/pgtap-core--0.95.0.sql
sudo cp sql/pgtap-schema--0.95.0.sql  /usr/share/postgresql/$1/extension/pgtap-schema--0.95.0.sql
  


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
