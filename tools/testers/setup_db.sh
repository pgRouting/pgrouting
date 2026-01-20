#!/bin/bash
# This file is part of the pgRouting project.
# Copyright (c) 2021-2026 pgRouting developers
# License: GPL-2 See https://github.com/pgRouting/pgrouting/blob/main/LICENSE

# Test pgRouting
# ------------------------------------------------------------------------------

set -e

DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

psql -p "$1" -U "$3"  -d "$2" -X -q --set client_min_messages=WARNING --set ON_ERROR_STOP=1 --pset pager=off \
    -c "CREATE EXTENSION IF NOT EXISTS pgtap; CREATE EXTENSION IF NOT EXISTS pgrouting WITH VERSION '${4}' CASCADE;"

psql -p "$1" -U "$3"  -d "$2" -X -q --set client_min_messages=WARNING --set ON_ERROR_STOP=1 --pset pager=off \
    -f "${DIR}/sampledata.pg" \
    -f "${DIR}/contractionHierarchies_data.pg" \
    -f "${DIR}/allpairs_tests.pg" \
    -f "${DIR}/solomon_100_rc101.data.pg" \
    -f "${DIR}/innerQuery.pg" \
    -f "${DIR}/innerQuery_old.pg" \
    -f "${DIR}/inner_styles.pg" \
    -f "${DIR}/old_inner_styles.pg" \
    -f "${DIR}/no_crash_test.pg" \
    -f "${DIR}/general_pgtap_tests.pg" \
    -f "${DIR}/no_crash_general.pg" \
    -f "${DIR}/dijkstra_pgtap_tests.pg" \
    -f "${DIR}/flow_pgtap_tests.pg" \
    -f "${DIR}/trsp_tests.pg" \
    -f "${DIR}/tsp_pgtap_tests.pg" \
    -f "${DIR}/types_check.pg" \
    -f "${DIR}/via_compare.pg" \
    -f "${DIR}/compare_dijkstra.pg" \
    -f "${DIR}/contraction_tapfuncs.pg" \
    -f "${DIR}/spanningtree.pg"
