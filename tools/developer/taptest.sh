#!/bin/bash

set -e

if [[ -z  $1 ]]; then
    echo "Usage:"
    echo "taptest.sh <pgtap directory> [<postgresql options>]";
    echo "Examples:"
    echo "taptest.sh withPoints/undirected_equalityDD.sql -p 5432"
    exit 1;
fi


DIR="$1"
shift
PGFLAGS=$*

echo "dir ${DIR}"
echo "pgflags ${PGFLAGS}"
QUIET="-v"
QUIET="-q"

PGDATABASE="___pgr___test___"
#PGRVERSION="3.3.0 3.2.1 3.2.0 3.1.3 3.1.2 3.1.1 3.1.0 3.0.5 3.0.4 3.0.3 3.0.2 3.0.1 3.0.0"
PGRVERSION="3.3.0"

cd tools/testers/

for v in ${PGRVERSION}
do
    echo "--------------------------"
    echo " Running with version ${v}"
    echo "--------------------------"

    dropdb --if-exists "${PGFLAGS}" "${PGDATABASE}"
    createdb "${PGFLAGS}" "${PGDATABASE}"

    psql "$PGFLAGS" -d "$PGDATABASE" -X -q --set client_min_messages=WARNING --set ON_ERROR_STOP=1 --pset pager=off \
        -c "CREATE EXTENSION IF NOT EXISTS pgtap; CREATE EXTENSION IF NOT EXISTS pgrouting WITH VERSION '${v}' CASCADE;"

    psql "${PGFLAGS}" -d "$PGDATABASE" -X -q --set client_min_messages=WARNING --set ON_ERROR_STOP=1 --pset pager=off \
        -f sampledata.sql \
        -f vrppdtw_data.sql \
        -f solomon_100_rc101.data.sql \
        -f innerQuery.sql \
        -f innerQuery_old.sql \
        -f inner_styles.sql \
        -f old_inner_styles.sql \
        -f no_crash_test.sql \
        -f alphaShapeTester.sql \
        -f binaryBreadthFirstSearch_pgtap_data.sql \
        -f general_pgtap_tests.sql \
        -f no_crash_general.sql \
        -f dijkstra_pgtap_tests.sql \
        -f tmp_net.sql \
        -f flow_pgtap_tests.sql \
        -f trsp_tests.sql \
        -f tsp_pgtap_tests.sql

    pg_prove "$QUIET" --normalize --directive --recurse "${PGFLAGS}"  -d "${PGDATABASE}" "../../pgtap/${DIR}"
    #dropdb --if-exists "${PGFLAGS}" "${PGDATABASE}"
done
