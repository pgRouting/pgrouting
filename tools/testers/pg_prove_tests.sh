#!/bin/bash
# ------------------------------------------------------------------------------
# Travis CI scripts 
# Copyright(c) pgRouting Contributors
#
# Test pgRouting
# ------------------------------------------------------------------------------

set -e 

PGUSER=$1
RELEASE_TYPE="$2"
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
#pg_prove ../../src/funnyDijkstra/test/pgtap/* -d $PGDATABASE

#TODO comment out peformance test before merging to MASTER


if [ $RELEASE_TYPE = "DEBUG" ]
then
    pg_prove ../../src/internalQueryTests/test/pgtap/* -d $PGDATABASE  -U $PGUSER
    echo "MADE TEST **********************"
fi
pg_prove ../../src/pickDeliver/test/pgtap/* -d $PGDATABASE  -U $PGUSER
pg_prove ../../src/astar/test/pgtap/* -d $PGDATABASE  -U $PGUSER


pg_prove ../../src/allpairs/test/pgtap/* -d $PGDATABASE  -U $PGUSER
pg_prove ../../src/alpha_shape/test/pgtap/* -d $PGDATABASE  -U $PGUSER
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
pg_prove ../../src/tsp/test/pgtap/* -d $PGDATABASE  -U $PGUSER

pg_prove ../../src/bd_astar/test/pgtap/* -d $PGDATABASE  -U $PGUSER
pg_prove ../../src/bd_dijkstra/test/pgtap/* -d $PGDATABASE  -U $PGUSER
pg_prove ../../src/convenience/test/pgtap/* -d $PGDATABASE  -U $PGUSER

pg_prove ../../src/tsp/test/performance/* -d $PGDATABASE  -U $PGUSER
pg_prove ../../src/contraction/test/pgtap/* -d $PGDATABASE  -U $PGUSER

if [ "$?" -ne 0 ]
then
    ERROR=1
fi

# Return success or failure
# ------------------------------------------------------------------------------
exit $ERROR
