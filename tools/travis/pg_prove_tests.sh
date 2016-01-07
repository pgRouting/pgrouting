#!/bin/sh
# ------------------------------------------------------------------------------
# Travis CI scripts 
# Copyright(c) pgRouting Contributors
#
# Test pgRouting
# ------------------------------------------------------------------------------

set -e 

PGUSER="postgres"
PGDATABASE="___pgr___test___"

# Define alias function for psql command
run_psql () {
    PGOPTIONS='--client-min-messages=warning' psql -U $PGUSER  -d $PGDATABASE -X -q -v ON_ERROR_STOP=1 --pset pager=off "$@"
    if [ "$?" -ne 0 ]
    then 
        echo "Test query failed: $@"
        ERROR=1
    fi 
}



echo "cd ./tools/testers/"
cd ./tools/testers/
echo "psql -f setup_db.sql"
run_psql -f setup_db.sql
echo "pg_prove ../../src/trsp/test/pgtap/*"
pg_prove ../../src/trsp/test/pgtap/* -d $PGDATABASE

if [ "$?" -ne 0 ]
then
    ERROR=1
fi

# Return success or failure
# ------------------------------------------------------------------------------
exit $ERROR
