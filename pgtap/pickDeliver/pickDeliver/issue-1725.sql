\i setup.sql

UPDATE edge_table SET cost = sign(cost), reverse_cost = sign(reverse_cost);
SELECT plan(1);
SET client_min_messages TO ERROR;


PREPARE missing_id_on_matrix AS
SELECT * FROM pgr_pickDeliver(
    $$ SELECT * FROM orders ORDER BY id $$,
    $$ SELECT * FROM vehicles $$,
    $$ SELECT * from pgr_dijkstraCostMatrix(
        ' SELECT * FROM edge_table ', ARRAY[3, 4, 5, 8, 9, 11])
    $$
);

SELECT throws_ok('missing_id_on_matrix', 'XX000', 'Unable to find node on matrix', 'Should throw: matrix is missing node 6');


SELECT finish();
