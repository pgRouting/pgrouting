#!/bin/sh
# ------------------------------------------------------------------------------
# Travis CI scripts 
# Copyright(c) pgRouting Contributors
#
# Test pgRouting
# ------------------------------------------------------------------------------

set -e 

PGUSER=$1
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
pg_prove ../../src/funnyDijkstra/test/pgtap/* -d $PGDATABASE

pg_prove ../../src/allpairs/test/pgtap/* -d $PGDATABASE  -U $PGUSER
pg_prove ../../src/apsp_johnson/test/pgtap/* -d $PGDATABASE  -U $PGUSER
pg_prove ../../src/apsp_warshall/test/pgtap/* -d $PGDATABASE  -U $PGUSER
pg_prove ../../src/ksp/test/pgtap/* -d $PGDATABASE  -U $PGUSER
pg_prove ../../src/topology/test/pgtap/* -d $PGDATABASE  -U $PGUSER
pg_prove ../../src/common/test/pgtap/* -d $PGDATABASE  -U $PGUSER
pg_prove ../../src/dijkstra/test/pgtap/* -d $PGDATABASE  -U $PGUSER
pg_prove ../../src/driving_distance/test/pgtap/* -d $PGDATABASE  -U $PGUSER
pg_prove ../../src/kdijkstra/test/pgtap/* -d $PGDATABASE  -U $PGUSER
pg_prove ../../src/withPoints/test/pgtap/* -d $PGDATABASE  -U $PGUSER
pg_prove ../../src/trsp/test/pgtap/* -d $PGDATABASE  -U $PGUSER

pg_prove ../../src/astar/test/pgtap/* -d $PGDATABASE  -U $PGUSER
pg_prove ../../src/bd_astar/test/pgtap/* -d $PGDATABASE  -U $PGUSER
pg_prove ../../src/bd_dijkstra/test/pgtap/* -d $PGDATABASE  -U $PGUSER
pg_prove ../../src/convinience/test/pgtap/* -d $PGDATABASE  -U $PGUSER

if [ "$?" -ne 0 ]
then
    ERROR=1
fi

# Return success or failure
# ------------------------------------------------------------------------------
exit $ERROR
