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

    prepare q1 AS
    SELECT seq, id1, id2, cost::TEXT FROM pgr_trsp(
        'select id, source::integer, target::integer, cost, reverse_cost from edge_table',
        1,     -- node_id of start
        5,     -- node_id of end
        true,  -- directed graph?
        true,  -- has_reverse_cost?
        null); -- no turn restrictions

    prepare q2 AS 
    SELECT seq-1, node::INTEGER, edge::INTEGER, cost::TEXT FROM pgr_dijkstra(
        'select id, source::integer, target::integer, cost, reverse_cost from edge_table',
        1, 5);

    SELECT set_eq('q2', 'q1', 'No turn restriction from 1 to 5 returns same as dijkstra');

    -- Finish the tests and clean up.
    SELECT * FROM finish();
    ROLLBACK;

