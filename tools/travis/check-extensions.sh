#!/bin/bash
# ------------------------------------------------------------------------------
# Travis CI scripts
# Copyright(c) pgRouting Contributors
#
# check pgtap & postgis on travis
# ------------------------------------------------------------------------------

set -e

POSTGRESQL_VERSION="$1"
DATABASE="$2"
PGUSER=postgres

sudo updatedb
echo "locate pgtap"
locate pgtap.control
echo "locate postgis"
locate postgis.control



cd ..
# Define alias function for psql command
run_psql () {
    PGOPTIONS='--client-min-messages=warning' psql -U $PGUSER  -e -X -v ON_ERROR_STOP=1 --pset pager=off "$@" -d $DATABASE
    if [ "$?" -ne 0 ]
    then
        echo "Test query failed: $@"
        ERROR=1
    fi
}

run_psql -c "SELECT version()"
run_psql -c "CREATE EXTENSION pgtap"
run_psql -c "SELECT pgtap_version()"
run_psql -c "CREATE EXTENSION postgis"
run_psql -c "SELECT postgis_full_version()"
run_psql -c "DROP EXTENSION pgtap"
run_psql -c "DROP EXTENSION postgis"

