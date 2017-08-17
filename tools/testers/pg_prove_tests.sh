#!/bin/bash
# ------------------------------------------------------------------------------
# Travis CI scripts
# Copyright(c) pgRouting Contributors
#
# Test pgRouting
# ------------------------------------------------------------------------------

set -e

PGUSER=$1
RELEASE_TYPE="b$2"
echo "RELEASE_TYPE" $RELEASE_TYPE

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


if [ $RELEASE_TYPE = "bDebug" ]
then
    pg_prove ../../src/internalQueryTests/test/pgtap/* -d $PGDATABASE  -U $PGUSER
    pg_prove ../../src/tsp/test/performance/* -d $PGDATABASE  -U $PGUSER
    echo "MADE TEST **********************"
fi

#pg_prove ../../src/dijkstraTRSP/test/pgtap/* -d $PGDATABASE -U $PGUSER

pg_prove ../../src/lineGraph/test/pgtap/* -d $PGDATABASE -U $PGUSER

pg_prove -d $PGDATABASE  -U $PGUSER ../../pgtap/*/*

if [ "$?" -ne 0 ]
then
    ERROR=1
fi

# Return success or failure
# ------------------------------------------------------------------------------
exit $ERROR
