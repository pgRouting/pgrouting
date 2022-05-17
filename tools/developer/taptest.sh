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
PGRVERSION="3.4.0 3.3.0 3.2.0 3.1.0 3.0.0"


for v in ${PGRVERSION}
do
    pushd tools/testers/
    echo "--------------------------"
    echo " Running with version ${v}"
    echo "--------------------------"

    dropdb --if-exists "${PGFLAGS}" "${PGDATABASE}"
    createdb "${PGFLAGS}" "${PGDATABASE}"

    psql "$PGFLAGS" -d "$PGDATABASE" -X -q --set client_min_messages=WARNING --set ON_ERROR_STOP=1 --pset pager=off \
        -c "CREATE EXTENSION IF NOT EXISTS pgtap; CREATE EXTENSION IF NOT EXISTS pgrouting WITH VERSION '${v}' CASCADE;"

    psql "${PGFLAGS}" -d "$PGDATABASE" -c "SELECT * FROM pgr_full_version();"

    psql "${PGFLAGS}" -d "$PGDATABASE" -X -q --set client_min_messages=WARNING --set ON_ERROR_STOP=1 --pset pager=off \
        -f sampledata_pgtap.sql \
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
        -f spanningtree.sql \
        -f types_check.sql \
        -f via_compare.sql \
        -f tsp_pgtap_tests.sql


    popd
    pg_prove "$QUIET" --normalize --directives --recurse "${PGFLAGS}"  -d "${PGDATABASE}" "pgtap/${DIR}"
    #pg_prove "$QUIET" --normalize --directive --recurse "${PGFLAGS}"  -d "${PGDATABASE}" "../../pgtap/"
    #dropdb --if-exists "${PGFLAGS}" "${PGDATABASE}"
done
