/*PGR-GNU*****************************************************************

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

------

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

********************************************************************PGR-GNU*/

\set ECHO none
\set QUIET 1
-- Turn off echo and keep things quiet.

-- Format the output for nice TAP.
\pset format unaligned
\pset tuples_only true
\pset pager
\set VERBOSITY terse

-- Revert all changes on failure.
\set ON_ERROR_ROLLBACK true
\set ON_ERROR_STOP true
\set QUIET 1

BEGIN;
    SELECT plan(1);
    UPDATE edge_table SET cost = cost + 0.001 * id * id, reverse_cost = reverse_cost + 0.001 * id * id;

    PREPARE q1 AS
    SELECT seq, id1, id2, cost::TEXT FROM pgr_trsp(
        'select id, source::integer, target::integer, cost, reverse_cost from edge_table',
        1,     -- edge_id for start
        0.5,   -- midpoint of edge
        6,     -- edge_id of route end
        0.5,   -- midpoint of edge
        true,  -- directed graph?
        true,  -- has_reverse_cost?
        null); -- no turn restrictions


    PREPARE q2 AS
    SELECT seq-1, node, edge, cost::TEXT FROM pgr_withPointsVia(
        'select id, source::integer, target::integer, cost, reverse_cost from edge_table',
        ARRAY[1, 6],
        ARRAY[0.5, 0.5]) WHERE edge != -2;

    SELECT set_eq('q2', 'q1', 'No turn restriction from 1 to 5 returns same as pgr_withPointsVia');

            -- Finish the tests and clean up.
    SELECT * FROM finish();
        ROLLBACK;


