#!/bin/bash
# ------------------------------------------------------------------------------
# Travis CI scripts
# Copyright(c) pgRouting Contributors
#
# Test pgRouting
# ------------------------------------------------------------------------------

set -e

PGUSER=$1
PGPORT=$2
RELEASE_TYPE="b$3"
echo "RELEASE_TYPE" $RELEASE_TYPE
if [ "b$PGPORT" = "b" ]
then
    PGPORT=""
else
    PGPORT=" -p $PGPORT"
fi
PGDATABASE="___pgr___test___"
echo $PGPORT



# Define alias function for psql command
run_psql () {
    PGOPTIONS='--client-min-messages=warning' psql $PGPORT -U $PGUSER  -d $PGDATABASE -X -q -v ON_ERROR_STOP=1 --pset pager=off "$@"
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
    pg_prove ../../src/internalQueryTests/test/pgtap/* $PGPORT -d $PGDATABASE  -U $PGUSER
    pg_prove ../../src/tsp/test/performance/* $PGPORT -d $PGDATABASE  -U $PGUSER
    echo "MADE TEST **********************"
fi


pg_prove $PGPORT -d $PGDATABASE  -U $PGUSER ../../pgtap/*/*


if [ "$?" -ne 0 ]
then
    ERROR=1
fi

# Return success or failure
# ------------------------------------------------------------------------------
exit $ERROR
