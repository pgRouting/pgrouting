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
    SELECT plan(7);

    UPDATE edge_table SET cost = cost + 0.001 * id * id, reverse_cost = reverse_cost + 0.001 * id * id;


    PREPARE q1 AS
    SELECT * from pgr_trsp(
        'select id as id, source, target, cost, reverse_cost from edge_table',
        1, 1,
        true, true);

    PREPARE q2 AS
    SELECT * from pgr_trsp(
        'select id as id, source::BIGINT, target, cost, reverse_cost from edge_table',
        1, 3,
        true, true);

    PREPARE q21 AS
    SELECT seq, id1, id2, cost::text from pgr_trsp(
        'select id as id, source::INTEGER, target::INTEGER,cost, reverse_cost from edge_table',
        1, 3,
        true, true);
        
    PREPARE q22 AS
    SELECT seq-1, node::INTEGER, edge::INTEGER, cost::text from pgr_dijkstra(
        'select id as id, source::INTEGER, target::INTEGER,cost, reverse_cost from edge_table',
        1, 3, true);

    PREPARE q31 AS
    SELECT seq, id1, id2, cost::text from pgr_trsp(
        'select id as id, source::INTEGER, target::INTEGER,cost, reverse_cost from edge_table',
        1, 3,
        true, true,
        'select 8::INTEGER as target_id, ''4''::TEXT as via_path,  100.2::FLOAT to_cost');

    PREPARE q32 AS
    SELECT (row_number() over() -1)::INTEGER, node::INTEGER, 
                        (CASE WHEN edge = -2 THEN -1 ELSE edge END)::INTEGER, cost::text
        FROM pgr_dijkstraViaVertex(
        'select id as id, source::INTEGER, target::INTEGER,cost, reverse_cost from edge_table',
        ARRAY[1, 10, 12, 4, 3],
        true) where edge != -1;


    SELECT is_empty('q1', '1: Directed: No path from 1 to 1');
    SELECT throws_ok('q2','XX000',
        'Error, columns ''source'', ''target'' must be of type int4, ''cost'' must be of type float8',
        '2: Directed: No path from 1 to 1');
    SELECT set_eq('q21','q22','3: Directed: without retrictions returns the same as pgr_dijkstra');
    SELECT set_eq('q31','q32','4: Directed: with retrictions returns expected path');

    -- Finish the tests and clean up.



    PREPARE q41 AS
    SELECT * from pgr_trsp(
        'select id as id, source::INTEGER, target::INTEGER,cost, reverse_cost from edge_table',
        1, 1,
        FALSE, true);

    PREPARE q51 AS
    SELECT seq, id1, id2, cost::text from pgr_trsp(
        'select id as id, source::INTEGER, target::INTEGER,cost, reverse_cost from edge_table',
        1, 3,
        FALSE, true);
        
    PREPARE q52 AS
    SELECT seq-1, node::INTEGER, edge::INTEGER, cost::text from pgr_dijkstra(
        'select id as id, source::INTEGER, target::INTEGER,cost, reverse_cost from edge_table',
        1, 3, FALSE);

    PREPARE q61 AS
    SELECT seq, id1, id2, cost::text from pgr_trsp(
        'select id as id, source::INTEGER, target::INTEGER,cost, reverse_cost from edge_table',
        1, 3,
        FALSE, true,
        'select 8::INTEGER as target_id, ''4''::TEXT as via_path,  100.2::FLOAT to_cost');

    PREPARE q62 AS
    SELECT (row_number() over() -1)::INTEGER, node::INTEGER, 
                        (CASE WHEN edge = -2 THEN -1 ELSE edge END)::INTEGER, cost::text
        FROM pgr_dijkstraViaVertex(
        'select id as id, source::INTEGER, target::INTEGER, cost, reverse_cost from edge_table',
        ARRAY[1, 2, 3],
        FALSE) where edge != -1;


    SELECT is_empty('q41', '5: Undirected: No path from 1 to 1');
    SELECT set_eq('q51','q52','6: Undirected: without retrictions returns the same as pgr_dijkstra');
    SELECT set_eq('q61','q62','7: Undirected: with retrictions returns expected path');

    -- Finish the tests and clean up.
    SELECT * FROM finish();
    ROLLBACK
