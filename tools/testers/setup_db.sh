#!/bin/bash
# ------------------------------------------------------------------------------
# Travis CI scripts
# Copyright(c) pgRouting Contributors
#
# Test pgRouting
# ------------------------------------------------------------------------------

set -e

psql -p "$1" -U "$3"  -d "$2" -X -q --set client_min_messages=WARNING --set ON_ERROR_STOP=1 --pset pager=off \
    -c "CREATE EXTENSION IF NOT EXISTS pgtap; CREATE EXTENSION IF NOT EXISTS pgrouting WITH VERSION '${4}' CASCADE;"

psql -p "$1" -U "$3"  -d "$2" -X -q --set client_min_messages=WARNING --set ON_ERROR_STOP=1 --pset pager=off \
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
    -f tsp_pgtap_tests.sql \
    -f spanningtree.sql
