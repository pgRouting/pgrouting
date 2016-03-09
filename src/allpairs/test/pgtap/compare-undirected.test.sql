\i setup.sql

SELECT plan(9);

-- TESTS WITH DIRECTED

-- all values must be >= 0
PREPARE q1 AS
SELECT *
FROM pgr_floydWarshall(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    FALSE
) WHERE agg_cost < 0;

PREPARE q2 AS
SELECT *
FROM pgr_floydWarshall(
    'SELECT id, source, target, cost FROM edge_table ORDER BY id',
    FALSE
) WHERE agg_cost < 0;

PREPARE q3 AS
SELECT *
FROM pgr_floydWarshall(
    'SELECT id, source, target, cost, -1::float as reverse_cost FROM edge_table ORDER BY id',
    FALSE
) WHERE agg_cost < 0;

PREPARE q4 AS
SELECT *
FROM pgr_floydWarshall(
    'SELECT id, source, target, -1::float as cost, reverse_cost FROM edge_table ORDER BY id',
    FALSE
) WHERE agg_cost < 0;


SELECT is_empty('q1', '1: No cost can be negative');
SELECT is_empty('q2', '2: No cost can be negative');
SELECT is_empty('q3', '3: No cost can be negative');
SELECT is_empty('q4', '4: No cost can be negative');

PREPARE q10 AS
SELECT *
FROM pgr_floydWarshall(
    'SELECT id, source, target, cost FROM edge_table ORDER BY id',
    FALSE
);

PREPARE q11 AS
SELECT *
FROM pgr_floydWarshall(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    FALSE
);

SELECT set_ne('q10', 'q11', '5: Results of with reverse_cost must be different of the one without it');

PREPARE q20 AS
SELECT  *
FROM pgr_dijkstraCost(
    'SELECT id, source, target, cost FROM edge_table ORDER BY id',
    ARRAY[1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17],
    ARRAY[1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17],
    FALSE
);

PREPARE q21 AS
SELECT  *
FROM pgr_dijkstraCost(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    ARRAY[1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17],
    ARRAY[1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17],
    FALSE
);

SELECT set_eq('q10', 'q20','6: With Out reverse_cost: Compare with (directed) pgr_dijkstraCost -> must give the same results');
SELECT set_eq('q11', 'q21','7: With reverse_cost: Compare with (directed) pgr_dijkstraCost -> must give the same results');


PREPARE q30 AS
SELECT  *
FROM pgr_johnson(
    'SELECT id, source, target, cost FROM edge_table ORDER BY id',
    FALSE
);

PREPARE q31 AS
SELECT  *
FROM pgr_johnson(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    FALSE
);

SELECT set_eq('q10', 'q30','8: With Out reverse_cost: Compare with (directed) pgr_johnson -> must give the same results');
SELECT set_eq('q11', 'q31','9: With reverse_cost: Compare with (directed) pgr_johnson -> must give the same results');




-- Finish the tests and clean up.
SELECT * FROM finish();
ROLLBACK;
