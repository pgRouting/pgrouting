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

/*
This test is for the equivalence of 
pgr_ kdijstraPath with pgr_dijkstra one to many
with
one to many dijkstra
*/

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


-- pgr_kdijstrapath
-- starts at: https://github.com/pgRouting/pgrouting/blob/master/src/kdijkstra/src/k_targets_sp.c#L1120

        SELECT plan(29);

        PREPARE doc1 AS
        SELECT * FROM pgr_kdijkstraPath(
            'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY ID',
            2, ARRAY[3], true, true);
        PREPARE q2 AS
        SELECT  seq, end_vid, node::INTEGER, edge::INTEGER, cost FROM pgr_dijkstra(
            'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY ID',
            2, ARRAY[3], true);

        SELECT set_eq('doc1', 'q2', 'From 2 to 3 returns the same result as pgr_dijkstra(one to many)');


        PREPARE q3 AS
        SELECT * FROM pgr_kdijkstraPath(
            'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY ID',
            2, ARRAY[2], true, true);
        PREPARE q4 AS
        SELECT 0::INTEGER AS seq, 2::INTEGER AS id1, 2::INTEGER as id2, -1::INTEGER as id3, 0::FLOAT as cost;
        SELECT set_eq('q3', 'q4','Gives a record when no path is found from v to v');

        PREPARE q5 AS
        SELECT id1,id2,id3,cost FROM pgr_kdijkstraPath(
            'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY ID',
            2, ARRAY[2,3], true, true);
        PREPARE q6 AS
        WITH the_union AS (
            (SELECT * FROM pgr_kdijkstraPath(
                    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY ID',
                    2, ARRAY[3], true, true))
            UNION ALL
            (SELECT 0::INTEGER AS seq, 2::INTEGER AS id1, 2::INTEGER as id2, -1::INTEGER as id3, 0::FLOAT as cost))
        SELECT id1,id2,id3,cost FROM the_union;
        SELECT set_eq('q5', 'q6','It is the union');


        PREPARE q7 AS
        SELECT * FROM pgr_kdijkstraPath(
            'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY ID',
            2, ARRAY[17], true, true);
        PREPARE q8 AS
        SELECT 0::INTEGER AS seq, 17::INTEGER AS id1, 0::INTEGER as id2, -1::INTEGER as id3, -1::FLOAT as cost;
        SELECT set_eq('q7', 'q8','Gives a record when no path is found from u to v');






        -- BASED ON THE CODE'S INTENTIONS

        PREPARE q9 AS
        SELECT * FROM pgr_kdijkstraPath(
            'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY ID',
            2, ARRAY[2, 18, 3], true, true);
        SELECT throws_ok('q9','XX000',
            'One of the target vertices was not found or several targets are the same.',
            'Only ONE target is not found: Code link: https://github.com/pgRouting/pgrouting/blob/master/src/kdijkstra/src/k_targets_sp.c#L790');

        PREPARE q10 AS
        SELECT * FROM pgr_kdijkstraPath(
            'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY ID',
            18, ARRAY[2, 7, 5, 7], true, true);
        SELECT throws_ok('q10','XX000',
            'One of the target vertices was not found or several targets are the same.',
            'Several targets are the same: Code link: https://github.com/pgRouting/pgrouting/blob/master/src/kdijkstra/src/k_targets_sp.c#L790');

        PREPARE q11 AS
        SELECT * FROM pgr_kdijkstraPath(
            'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY ID',
            2, ARRAY[18, 19, 20], true, true);
        SELECT throws_ok('q11','XX000',
            'None of the target vertices has been found; aborting!',
            'None of the targets are found Code link: https://github.com/pgRouting/pgrouting/blob/master/src/kdijkstra/src/k_targets_sp.c#L794');

        PREPARE q12 AS
        SELECT * FROM pgr_kdijkstraPath(
            'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY ID',
            18, ARRAY[2], true, true);
        SELECT throws_ok('q12','XX000',
            'Start vertex was not found.',
            'Start vertex does not exist in the edges: Code Link: https://github.com/pgRouting/pgrouting/blob/master/src/kdijkstra/src/k_targets_sp.c#L784');




         
        -- BASED ON THE DOCUMENTATION
        PREPARE doc11 AS
        SELECT * FROM pgr_kdijkstraPath(
            'SELECT id::BIGINT, source, target, cost, reverse_cost FROM edge_table ORDER BY ID',
            2, ARRAY[3], true, true);
        SELECT throws_ok('doc11','XX000',
            'Column id must be of type int4',
            'id is not int');

        PREPARE doc12 AS
        SELECT * FROM pgr_kdijkstraPath(
            'SELECT id, source::BIGINT, target, cost, reverse_cost FROM edge_table ORDER BY ID',
            2, ARRAY[3], true, true);
        SELECT throws_ok('doc12','XX000',
            'Error, columns ''source'', ''target'' must be of type int4, ''cost'' must be of type float8',
            'source is not int');

        PREPARE doc13 AS
        SELECT * FROM pgr_kdijkstraPath(
            'SELECT id, source, target::BIGINT, cost, reverse_cost FROM edge_table ORDER BY ID',
            2, ARRAY[3], true, true);
        SELECT throws_ok('doc13','XX000',
            'Error, columns ''source'', ''target'' must be of type int4, ''cost'' must be of type float8',
            'target is not int');

        PREPARE doc14 AS
        SELECT * FROM pgr_kdijkstraPath(
            'SELECT id, source, target, cost::INTEGER, reverse_cost FROM edge_table ORDER BY ID',
            2, ARRAY[3], true, true);
        SELECT throws_ok('doc14','XX000',
            'Error, columns ''source'', ''target'' must be of type int4, ''cost'' must be of type float8',
            'cost is not float');

        PREPARE doc15 AS
        SELECT * FROM pgr_kdijkstraPath(
            'SELECT id, source, target, cost, reverse_cost::INTEGER FROM edge_table ORDER BY ID',
            2, ARRAY[3], true, true);
        SELECT throws_ok('doc15','XX000',
            'Error, columns ''reverse_cost'' must be of type float8',
            'reverse_cost is not float');


        SELECT throws_ok(
            '
            SELECT * FROM pgr_kdijkstraPath(
                ''SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id'',
                2, 3, true, true);
            '
            '42883','function pgr_kdijkstrapath(unknown, integer, integer, boolean, boolean) does not exist',
            'Throws because an array was not given');

        SELECT throws_ok(
            '
            SELECT * FROM pgr_kdijkstraPath(
                ''SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id'',
                2::BIGINT, ARRAY[3], true, true);
            ',
            '42883','function pgr_kdijkstrapath(unknown, bigint, integer[], boolean, boolean) does not exist',
            'Throws because a NOT int was was given as starting vertex');

        -- BASED ON THE CODE'S INTENTIONS

        -- this code never gets executed
        -- https://github.com/pgRouting/pgrouting/blob/master/src/kdijkstra/src/k_targets_sp.c#L121
        SELECT throws_ok(
            '
            SELECT * FROM pgr_kdijkstraPath(
                ''SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id'',
                2, ARRAY[3::BIGINT], true, true);
            ',
            '42883','function pgr_kdijkstrapath(unknown, integer, bigint[], boolean, boolean) does not exist',
            'Throws because the array has data that is not integer');

        SELECT throws_ok(
            '
            SELECT * FROM pgr_kdijkstraPath(
                ''SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id'',
                2, ARRAY[[1,2],[3,4]], true, true);
            ',
            'XX000','target must be integer[]',
            'Throws because its not a 1 dimention array: https://github.com/pgRouting/pgrouting/blob/master/src/kdijkstra/src/k_targets_sp.c#L131');

        SELECT throws_ok(
            '
            SELECT * FROM pgr_kdijkstraPath(
                ''SELECT source, target, cost, reverse_cost FROM edge_table ORDER BY id'',
                2, ARRAY[3], true, true);
            ',
            'XX000','Error, query must return columns ''id'', ''source'', ''target'' and ''cost''',
            'Throws because its missing id: https://github.com/pgRouting/pgrouting/blob/master/src/kdijkstra/src/k_targets_sp.c#L214');

        SELECT throws_ok(
            '
            SELECT * FROM pgr_kdijkstraPath(
                ''SELECT id, target, cost, reverse_cost FROM edge_table ORDER BY id'',
                2, ARRAY[3], true, true);
            ',
            'XX000','Error, query must return columns ''id'', ''source'', ''target'' and ''cost''',
            'Throws because its missing source: https://github.com/pgRouting/pgrouting/blob/master/src/kdijkstra/src/k_targets_sp.c#L214');

        SELECT throws_ok(
            '
            SELECT * FROM pgr_kdijkstraPath(
                ''SELECT id, source, cost, reverse_cost FROM edge_table ORDER BY id'',
                2, ARRAY[3], true, true);
            ',
            'XX000','Error, query must return columns ''id'', ''source'', ''target'' and ''cost''',
            'Throws because its missing target: https://github.com/pgRouting/pgrouting/blob/master/src/kdijkstra/src/k_targets_sp.c#L214');

        SELECT throws_ok(
            '
            SELECT * FROM pgr_kdijkstraPath(
                ''SELECT id, target, source, reverse_cost FROM edge_table ORDER BY id'',
                2, ARRAY[3], true, true);
            ',
            'XX000','Error, query must return columns ''id'', ''source'', ''target'' and ''cost''',
            'Throws because its missing cost: https://github.com/pgRouting/pgrouting/blob/master/src/kdijkstra/src/k_targets_sp.c#L214');


        SELECT throws_ok(
            '
            SELECT * FROM pgr_kdijkstraPath(
                ''SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id'',
                2, ARRAY[3]::BIGINT[], true, true);
            ',
            '42883','function pgr_kdijkstrapath(unknown, integer, bigint[], boolean, boolean) does not exist',
            'Throws because the arrays are of big int');

        SELECT throws_ok(
            '
            SELECT * FROM pgr_kdijkstraPath(
                ''SELECT id, source, target, cost FROM edge_table ORDER BY id'',
                2, ARRAY[3], true, true);
            ',
            '42883','Error, reverse_cost is used, but query did''t return ''reverse_cost'' column',
            'Throws because unsalvable contradiction: https://github.com/pgRouting/pgrouting/blob/master/src/kdijkstra/src/k_targets_sp.c#L237');

        SELECT lives_ok(
            '
            SELECT * FROM pgr_kdijkstraPath(
                ''SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id'',
                2, ARRAY[3], true, false);
            ',
            'Does not throw when salvable contradiction');


        UPDATE edge_table SET source = NULL WHERE id = 1;

        SELECT throws_ok(
            '
            SELECT * FROM pgr_kdijkstraPath(
                ''SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id'',
                2, ARRAY[3], true, true);
            ',
            'XX000','source contains a null value',
            'Throws because source has a null: https://github.com/pgRouting/pgrouting/blob/master/src/kdijkstra/src/k_targets_sp.c#L267');

        SELECT throws_ok(
            '
            SELECT * FROM pgr_kdijkstraPath(
                ''SELECT source AS id, source, target, cost, reverse_cost FROM edge_table ORDER BY id'',
                2, ARRAY[3], true, true);
            ',
            'XX000','id contains a null value',
            'Throws because id has a null: https://github.com/pgRouting/pgrouting/blob/master/src/kdijkstra/src/k_targets_sp.c#L267');



        UPDATE edge_table SET source = 1, target = NULL WHERE id = 1;

        SELECT throws_ok(
            '
            SELECT * FROM pgr_kdijkstraPath(
                ''SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id'',
                2, ARRAY[3], true, true);
            ',
            'XX000','target contains a null value',
            'Throws because target has a null: https://github.com/pgRouting/pgrouting/blob/master/src/kdijkstra/src/k_targets_sp.c#L272');



        UPDATE edge_table SET target = 1, cost = NULL WHERE id = 1;

        SELECT throws_ok(
            '
            SELECT * FROM pgr_kdijkstraPath(
                ''SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id'',
                2, ARRAY[3], true, true);
            ',
            'XX000','cost contains a null value',
            'Throws because cost has a null: https://github.com/pgRouting/pgrouting/blob/master/src/kdijkstra/src/k_targets_sp.c#L277');



        UPDATE edge_table SET cost = 1, reverse_cost = NULL WHERE id = 1;

        SELECT throws_ok(
            '
            SELECT * FROM pgr_kdijkstraPath(
                ''SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id'',
                2, ARRAY[3], true, true);
            ',
            'XX000','reverse_cost contains a null value',
            'Throws because reverse_cost has a null: https://github.com/pgRouting/pgrouting/blob/master/src/kdijkstra/src/k_targets_sp.c#L285');

        UPDATE edge_table SET reverse_cost = 1 WHERE id = 1;



        -- Finish the tests and clean up.
        SELECT * FROM finish();
        ROLLBACK;
