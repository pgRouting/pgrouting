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
     SELECT plan(11);

-- all this queries are equivalent (give the same results)
PREPARE q00 AS
SELECT  id2 FROM pgr_dijkstra(
    'SELECT id AS id, source::int4, target::int4,
    CASE WHEN cost<=0 THEN 999 ELSE cost END AS cost
    FROM edge_table ORDER BY id',
    7, 11, false, false);

PREPARE q0 AS
SELECT  id1 FROM pgr_dijkstra(
    'SELECT id AS id, source::int4, target::int4,
    CASE WHEN cost<=0 THEN 999 ELSE cost END AS cost
    FROM edge_table ORDER BY id',
    7, 11, false, false);

PREPARE q1 AS
SELECT seq, id1, id2, cost FROM pgr_dijkstra(
    'SELECT id AS id, source::int4, target::int4,
    CASE WHEN cost<=0 THEN 999 ELSE cost END AS cost
    FROM edge_table ORDER BY id',
    7, 11, false, false);

PREPARE q2 AS
SELECT seq - 1, node::INTEGER, edge::INTEGER, cost FROM pgr_dijkstra(
    'SELECT id AS id, source::int4, target::int4, 
    CASE WHEN cost<=0 THEN 999 ELSE cost END AS cost 
    FROM edge_table ORDER BY id', 
    7, 11, false);


PREPARE q3 AS
SELECT seq - 1, node::INTEGER, edge::INTEGER, cost FROM pgr_dijkstra(
    'SELECT id AS id, source::int4, target::int4, 
    CASE WHEN cost<=0 THEN 999 ELSE cost END AS cost 
    FROM edge_table ORDER BY id', 
    7, 11, false);

PREPARE q4 AS
SELECT seq - 1, node::INTEGER, edge::INTEGER, cost FROM pgr_dijkstra(
        'SELECT id, source, target, cost 
        FROM edge_table ORDER BY id', 
        7, 11, false);


PREPARE q5 AS
SELECT seq - 1, node::INTEGER, edge::INTEGER, cost FROM pgr_dijkstra(
        'SELECT id, source, target, cost,
        -1 as reverse_cost
        FROM edge_table ORDER BY id', 
        7, 11, false);

PREPARE q6 AS
SELECT seq - 1, node::INTEGER, edge::INTEGER, cost FROM pgr_dijkstra(
        'SELECT id, source, target, cost 
        FROM edge_table ORDER BY id', 
        ARRAY[7], 11, false);

PREPARE q7 AS
SELECT seq - 1, node::INTEGER, edge::INTEGER, cost FROM pgr_dijkstra(
        'SELECT id, source, target, cost 
        FROM edge_table ORDER BY id', 
        7, ARRAY[11], false);

PREPARE q8 AS
SELECT seq - 1, node::INTEGER, edge::INTEGER, cost FROM pgr_dijkstra(
        'SELECT id, source, target, cost 
        FROM edge_table ORDER BY id', 
        ARRAY[7], ARRAY[11], false);

PREPARE q9 AS
SELECT seq, id1, id2, cost FROM pgr_dijkstra(
    'SELECT id AS id, source::int4, target::int4,
    cost
    FROM edge_table ORDER BY id',
    7, 11, false, false);



    SELECT set_eq( 'q0', ARRAY[7, 8, 5, 10, 11], '1');
    SELECT set_eq( 'q00', ARRAY[6, 7, 10, 12, -1], '2');
    SELECT set_eq( 'q1', 'q2', '3');
    SELECT set_eq( 'q1', 'q3', '4');
    SELECT set_eq( 'q1', 'q4', '5');
    SELECT set_eq( 'q1', 'q5', '6');
    SELECT set_eq( 'q1', 'q6', '7');
    SELECT set_eq( 'q1', 'q7', '8');
    SELECT set_eq( 'q1', 'q8', '9');
    SELECT set_eq( 'q1', 'q9', '10');

PREPARE q50 AS
SELECT seq, id1, id2, cost FROM pgr_dijkstra(
    'SELECT id AS id, source::BIGINT, target::BIGINT,
    CASE WHEN cost<=0 THEN 999 ELSE cost END AS cost
    FROM edge_table ORDER BY id',
    7, 11, false, false);

SELECT throws_ok('q50',
    'XX000',
    'Error, columns ''source'', ''target'' must be of type int4, ''cost'' must be of type float8',
    '11');

    -- Finish the tests and clean up.
    SELECT * FROM finish();
    ROLLBACK;

