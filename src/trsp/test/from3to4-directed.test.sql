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
    SELECT plan(3);

    UPDATE edge_table SET cost = cost + 0.001 * id * id, reverse_cost = reverse_cost + 0.001 * id * id;


    PREPARE q1 AS
    SELECT * from pgr_trsp(
        'select id as id, source::integer, target::integer,cost, reverse_cost from edge_table',
        1, 1,
        true, true);

    PREPARE q21 AS
    SELECT seq, id1, id2, cost::text from pgr_trsp(
        'select id as id, source::integer, target::integer,cost, reverse_cost from edge_table',
        3, 4,
        true, true);
        
    PREPARE q22 AS
    SELECT seq-1, node::INTEGER, edge::INTEGER, cost::text from pgr_dijkstra(
        'select id as id, source::integer, target::integer,cost, reverse_cost from edge_table',
        3, 4, true);

    PREPARE q31 AS
    SELECT seq, id1, id2, cost::text from pgr_trsp(
        'select id as id, source::integer, target::integer,cost, reverse_cost from edge_table',
        3, 4,
        true, true,
        'select 9::INTEGER as target_id, ''5''::TEXT as via_path,  100.2::FLOAT to_cost');

    PREPARE q32 AS
    SELECT (row_number() over() -1)::INTEGER, node::INTEGER, 
                        (CASE WHEN edge = -2 THEN -1 ELSE edge END)::INTEGER, cost::text
        FROM pgr_dijkstraViaVertex(
        'select id as id, source::integer, target::integer,cost, reverse_cost from edge_table',
        ARRAY[3, 2, 4],
        true) where edge != -1;


    SELECT is_empty('q1', '1: No path from 1 to 1');
    SELECT set_eq('q21','q22','2: without retrictions returns the same as pgr_dijkstra');
    SELECT set_eq('q31','q32','3: with retrictions returns expected path');

    -- Finish the tests and clean up.

    SELECT * FROM finish();


    ROLLBACK
