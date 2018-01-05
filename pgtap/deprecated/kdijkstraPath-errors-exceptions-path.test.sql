
/*
This test is for the equivalence of
pgr_ kdijstraPath with pgr_dijkstra one to many
with
one to many dijkstra
*/

\i setup.sql
SET client_min_messages = WARNING;


SELECT plan(29);

PREPARE doc1 AS
SELECT * FROM pgr_kdijkstraPath(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table ORDER BY ID',
    2, ARRAY[3], true, true);
PREPARE q2 AS
SELECT seq-1,  end_vid::INTEGER, node::INTEGER, edge::INTEGER, cost FROM pgr_dijkstra(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table ORDER BY ID',
    2, ARRAY[3], true);

SELECT set_eq('doc1', 'q2', 'From 2 to 3 returns the same result as pgr_dijkstra(one to many)');


PREPARE q3 AS
SELECT * FROM pgr_kdijkstraPath(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table ORDER BY ID',
    2, ARRAY[2], true, true);
PREPARE q4 AS
SELECT 0::INTEGER AS seq, 2::INTEGER AS id1, 2::INTEGER as id2, -1::INTEGER as id3, 0::FLOAT as cost;
SELECT set_eq('q3', 'q4','Gives a record when no path is found from v to v');

PREPARE q5 AS
SELECT id1,id2,id3,cost FROM pgr_kdijkstraPath(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table ORDER BY ID',
    2, ARRAY[2,3], true, true);
PREPARE q6 AS
WITH the_union AS (
    (SELECT * FROM pgr_kdijkstraPath(
            'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table ORDER BY ID',
            2, ARRAY[3], true, true))
    UNION ALL
    (SELECT 0::INTEGER AS seq, 2::INTEGER AS id1, 2::INTEGER as id2, -1::INTEGER as id3, 0::FLOAT as cost))
SELECT id1,id2,id3,cost FROM the_union;
SELECT set_eq('q5', 'q6','It is the union');


PREPARE q7 AS
SELECT * FROM pgr_kdijkstraPath(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table ORDER BY ID',
    2, ARRAY[17], true, true);
PREPARE q8 AS
SELECT 0::INTEGER AS seq, 17::INTEGER AS id1, 0::INTEGER as id2, -1::INTEGER as id3, -1::FLOAT as cost;
SELECT set_eq('q7', 'q8','Gives a record when no path is found from u to v');






-- BASED ON THE CODE'S INTENTIONS

--  Code link: https://github.com/pgRouting/pgrouting/blob/master/src/kdijkstra/src/k_targets_sp.c#L790');
PREPARE q9 AS
SELECT * FROM pgr_kdijkstraPath(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table ORDER BY ID',
    2, ARRAY[2, 18, 3], true, true);
SELECT lives_ok('q9',
    'Not documented: When a target is not found there is no path, So returns -1 in cost');

-- Code link: https://github.com/pgRouting/pgrouting/blob/master/src/kdijkstra/src/k_targets_sp.c#L790');
PREPARE q10 AS
SELECT * FROM pgr_kdijkstraPath(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table ORDER BY ID',
    18, ARRAY[2, 7, 5, 7], true, true);
SELECT lives_ok('q10',
    'Several targets are the same: returns all values');

-- Code link: https://github.com/pgRouting/pgrouting/blob/master/src/kdijkstra/src/k_targets_sp.c#L794');
PREPARE q11 AS
SELECT * FROM pgr_kdijkstraPath(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table ORDER BY ID',
    2, ARRAY[18, 19, 20, 2], true, true);
SELECT lives_ok('q11',
    'None of the target vertices has been found; all costs must have a -1 (except for v,v)');

-- Code Link: https://github.com/pgRouting/pgrouting/blob/master/src/kdijkstra/src/k_targets_sp.c#L784');
PREPARE q12 AS
SELECT * FROM pgr_kdijkstraPath(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table ORDER BY ID',
    18, ARRAY[2], true, true);
SELECT lives_ok('q12',
    'Start vertex does not exist in the edges: all cost must have a -1');





-- BASED ON THE DOCUMENTATION
--Throws because id is not int');
PREPARE doc11 AS
SELECT * FROM pgr_kdijkstraPath(
    'SELECT id::BIGINT, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table ORDER BY ID',
    2, ARRAY[3], true, true);
SELECT throws_ok('doc11', 'XX000');

PREPARE doc12 AS
SELECT * FROM pgr_kdijkstraPath(
    'SELECT id::INTEGER, source::BIGINT, target::INTEGER, cost, reverse_cost FROM edge_table ORDER BY ID',
    2, ARRAY[3], true, true);
SELECT throws_ok('doc12','XX000',
    'Error, columns ''source'', ''target'' must be of type int4, ''cost'' must be of type float8',
    '10: Throws when source is not int');

PREPARE doc13 AS
SELECT * FROM pgr_kdijkstraPath(
    'SELECT id::INTEGER, source::INTEGER, target::BIGINT, cost, reverse_cost FROM edge_table ORDER BY ID',
    2, ARRAY[3], true, true);
SELECT throws_ok('doc13','XX000',
    'Error, columns ''source'', ''target'' must be of type int4, ''cost'' must be of type float8',
    '11: Throws when target is not int');

PREPARE doc14 AS
SELECT * FROM pgr_kdijkstraPath(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::INTEGER, reverse_cost FROM edge_table ORDER BY ID',
    2, ARRAY[3], true, true);
SELECT throws_ok('doc14','XX000',
    'Error, columns ''source'', ''target'' must be of type int4, ''cost'' must be of type float8',
    '12: Throws when cost is not float');

PREPARE doc15 AS
SELECT * FROM pgr_kdijkstraPath(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost::INTEGER FROM edge_table ORDER BY ID',
    2, ARRAY[3], true, true);
SELECT throws_ok('doc15','XX000');


SELECT throws_ok(
    '
    SELECT * FROM pgr_kdijkstraPath(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table ORDER BY id'',
        2, 3, true, true);
    ',
    '42883','function pgr_kdijkstrapath(unknown, integer, integer, boolean, boolean) does not exist',
    '14: Throws because an array was not given');

SELECT throws_ok(
    '
    SELECT * FROM pgr_kdijkstraPath(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table ORDER BY id'',
        2::BIGINT, ARRAY[3], true, true);
    ',
    '42883','function pgr_kdijkstrapath(unknown, bigint, integer[], boolean, boolean) does not exist',
    '15: Throws because a NOT int was was given as starting vertex');

-- BASED ON THE CODE'S INTENTIONS

-- this code never gets executed
-- https://github.com/pgRouting/pgrouting/blob/master/src/kdijkstra/src/k_targets_sp.c#L121
SELECT throws_ok(
    '
    SELECT * FROM pgr_kdijkstraPath(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table ORDER BY id'',
        2, ARRAY[3::BIGINT], true, true);
    ',
    '42883','function pgr_kdijkstrapath(unknown, integer, bigint[], boolean, boolean) does not exist',
    '16: Throws because the array has data that is not integer');

-- Throws because its not a 1 dimension array: https://github.com/pgRouting/pgrouting/blob/master/src/kdijkstra/src/k_targets_sp.c#L131');
SELECT throws_ok(
    '
    SELECT * FROM pgr_kdijkstraPath(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table ORDER BY id'',
        2, ARRAY[[1,2],[3,4]], true, true);
    ', 'XX000');

--   'Throws because its missing id: https://github.com/pgRouting/pgrouting/blob/master/src/kdijkstra/src/k_targets_sp.c#L214');
SELECT throws_ok(
    '
    SELECT * FROM pgr_kdijkstraPath(
        ''SELECT source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table ORDER BY id'',
        2, ARRAY[3], true, true);
    ',
    'XX000');

--    'Throws because its missing source: https://github.com/pgRouting/pgrouting/blob/master/src/kdijkstra/src/k_targets_sp.c#L214');
SELECT throws_ok(
    '
    SELECT * FROM pgr_kdijkstraPath(
        ''SELECT id::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table ORDER BY id'',
        2, ARRAY[3], true, true);
    ',
    'XX000');

--    'Throws because its missing target: https://github.com/pgRouting/pgrouting/blob/master/src/kdijkstra/src/k_targets_sp.c#L214');
SELECT throws_ok(
    '
    SELECT * FROM pgr_kdijkstraPath(
        ''SELECT id::INTEGER, source::INTEGER, cost, reverse_cost FROM edge_table ORDER BY id'',
        2, ARRAY[3], true, true);
    ',
    'XX000');

--    'Throws because its missing cost: https://github.com/pgRouting/pgrouting/blob/master/src/kdijkstra/src/k_targets_sp.c#L214');
SELECT throws_ok(
    '
    SELECT * FROM pgr_kdijkstraPath(
        ''SELECT id::INTEGER, target::INTEGER, source::INTEGER, reverse_cost FROM edge_table ORDER BY id'',
        2, ARRAY[3], true, true);
    ',
    'XX000');


SELECT throws_ok(
    '
    SELECT * FROM pgr_kdijkstraPath(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table ORDER BY id'',
        2, ARRAY[3]::BIGINT[], true, true);
    ',
    '42883','function pgr_kdijkstrapath(unknown, integer, bigint[], boolean, boolean) does not exist',
    '22: Throws because the arrays are of big int');

SELECT throws_ok(
    '
    SELECT * FROM pgr_kdijkstraPath(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost FROM edge_table ORDER BY id'',
        2, ARRAY[3], true, true);
    ',
    'XX000','Error, reverse_cost is used, but query did''t return ''reverse_cost'' column',
    '23: Throws because unsalvable contradiction: https://github.com/pgRouting/pgrouting/blob/master/src/kdijkstra/src/k_targets_sp.c#L237');

SELECT lives_ok(
    '
    SELECT * FROM pgr_kdijkstraPath(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table ORDER BY id'',
        2, ARRAY[3], true, false);
    ',
    '24: Does not throw when salvable contradiction');


-- TROWS because of null values
-- https://github.com/pgRouting/pgrouting/blob/master/src/kdijkstra/src/k_targets_sp.c#L285');
UPDATE edge_table SET source = NULL WHERE id = 1;

SELECT throws_ok(
    '
    SELECT * FROM pgr_kdijkstraPath(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table ORDER BY id'',
        2, ARRAY[3], true, true);
    ',
    'XX000');

SELECT throws_ok(
    '
    SELECT * FROM pgr_kdijkstraPath(
        ''SELECT source::INTEGER AS id, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table ORDER BY id'',
        2, ARRAY[3], true, true);
    ',
    'XX000');



UPDATE edge_table SET source = 1, target = NULL WHERE id = 1;

SELECT throws_ok(
    '
    SELECT * FROM pgr_kdijkstraPath(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table ORDER BY id'',
        2, ARRAY[3], true, true);
    ',
    'XX000');



UPDATE edge_table SET target = 1, cost = NULL WHERE id = 1;

SELECT throws_ok(
    '
    SELECT * FROM pgr_kdijkstraPath(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table ORDER BY id'',
        2, ARRAY[3], true, true);
    ',
    'XX000');



UPDATE edge_table SET cost = 1, reverse_cost = NULL WHERE id = 1;

SELECT throws_ok(
    '
    SELECT * FROM pgr_kdijkstraPath(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table ORDER BY id'',
        2, ARRAY[3], true, true);
    ',
    'XX000');

UPDATE edge_table SET reverse_cost = 1 WHERE id = 1;



-- Finish the tests and clean up.
SELECT * FROM finish();
ROLLBACK;
