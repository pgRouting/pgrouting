#!/bin/bash
# ------------------------------------------------------------------------------
# /*PGR-GNU*****************************************************************
# File: setup_db.sh
# Copyright (c) 2021 pgRouting developers
# Mail: project@pgrouting.org
# ------
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
# ********************************************************************PGR-GNU*/
# Travis CI scripts
# Copyright(c) pgRouting Contributors
#
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
