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

psql -p "$1" -U "$3"  -d "$2" -X -q --set client_min_messages=WARNING --set ON_ERROR_STOP=1 --pset pager=off \
    -c "CREATE EXTENSION IF NOT EXISTS pgtap; CREATE EXTENSION IF NOT EXISTS pgrouting WITH VERSION '${4}' CASCADE;"

psql -p "$1" -U "$3"  -d "$2" -X -q --set client_min_messages=WARNING --set ON_ERROR_STOP=1 --pset pager=off \
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
    -f tsp_pgtap_tests.sql \
    -f astar_pgtap_tests.sql \
    -f types_check.sql \
    -f via_compare.sql \
    -f spanningtree.sql
