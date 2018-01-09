
\i setup.sql
SET client_min_messages=WARNING;

SELECT plan(16);

-- TESTS WITH DIRECTED

-- all values must be >= 0
PREPARE q1 AS
SELECT *
FROM pgr_apspWarshall(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table ORDER BY id',
    TRUE, TRUE
) WHERE cost < 0;

PREPARE q2 AS
SELECT *
FROM pgr_apspWarshall(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost FROM edge_table ORDER BY id',
    TRUE, FALSE
) WHERE cost < 0;

PREPARE q3 AS
SELECT *
FROM pgr_apspWarshall(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, -1::float as reverse_cost FROM edge_table ORDER BY id',
    TRUE, TRUE
) WHERE cost < 0;

PREPARE q4 AS
SELECT *
FROM pgr_apspWarshall(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, -1::float as cost, reverse_cost FROM edge_table ORDER BY id',
    TRUE, TRUE
) WHERE cost < 0;


SELECT is_empty('q1', '1: No cost can be negative');
SELECT is_empty('q2', '2: No cost can be negative');
SELECT is_empty('q3', '3: No cost can be negative');
SELECT is_empty('q4', '4: No cost can be negative');

PREPARE q10 AS
SELECT id1, id2, cost
FROM pgr_apspWarshall(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost FROM edge_table ORDER BY id',
    TRUE, FALSE
);

PREPARE q11 AS
SELECT id1, id2, cost
FROM pgr_apspWarshall(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table ORDER BY id',
    TRUE, TRUE
);

SELECT set_ne('q10', 'q11', '5: Results of with reverse_cost must be different of the one without it');

PREPARE q20 AS
SELECT  start_vid::integer AS id1, end_vid::integer AS id2, agg_cost AS cost
FROM pgr_dijkstraCost(
    'SELECT id, source, target, cost FROM edge_table ORDER BY id',
    ARRAY[1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17],
    ARRAY[1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17]
);

PREPARE q21 AS
SELECT  start_vid::integer AS id1, end_vid::integer AS id2, agg_cost AS cost
FROM pgr_dijkstraCost(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    ARRAY[1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17],
    ARRAY[1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17]
);

SELECT set_eq('q10', 'q20','6: With Out reverse_cost: Compare with (directed) pgr_dijkstraCost -> must give the same results');
SELECT set_eq('q11', 'q21','7: With reverse_cost: Compare with (directed) pgr_dijkstraCost -> must give the same results');


PREPARE q30 AS
SELECT  start_vid::integer AS id1, end_vid::integer AS id2, agg_cost AS cost
FROM pgr_johnson(
    'SELECT id, source, target, cost FROM edge_table ORDER BY id',
    TRUE
);

PREPARE q31 AS
SELECT  start_vid::integer AS id1, end_vid::integer AS id2, agg_cost AS cost
FROM pgr_johnson(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id'
);

SELECT set_eq('q10', 'q30','8: With Out reverse_cost: Compare with (directed) pgr_johnson -> must give the same results');
SELECT set_eq('q11', 'q31','9: With reverse_cost: Compare with (directed) pgr_johnson -> must give the same results');


PREPARE q40 AS
SELECT  start_vid::integer AS id1, end_vid::integer AS id2, agg_cost AS cost
FROM pgr_floydWarshall(
    'SELECT id, source, target, cost FROM edge_table ORDER BY id'
);

PREPARE q41 AS
SELECT  start_vid::integer AS id1, end_vid::integer AS id2, agg_cost AS cost
FROM pgr_floydWarshall(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id'
);

SELECT set_eq('q10', 'q40','10: With Out reverse_cost: Compare with (directed) pgr_floydWarshall -> must give the same results');
SELECT set_eq('q11', 'q41','11: With reverse_cost: Compare with (directed) pgr_floydWarshall -> must give the same results');

PREPARE q50 AS
SELECT  id1, id2, cost
FROM pgr_apspJohnson(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost FROM edge_table ORDER BY id'
);

PREPARE q51 AS
SELECT  id1, id2, cost
FROM pgr_apspJohnson(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table ORDER BY id'
);

SELECT set_eq('q10', 'q50','12: With Out reverse_cost: Compare with pgr_apspJohnson -> must give the same results');
SELECT set_ne('q11', 'q51','13: With reverse_cost: Compare with pgr_apspJohnson -> Results must be different (pgr_apspJohnson only workds without reverse_cost');

-- errors:

-- flags
SELECT throws_ok(
    'SELECT * FROM pgr_apspWarshall(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table ORDER BY id''
    )','42883','function pgr_apspwarshall(unknown) does not exist',
    '14: Documentation says it does needs 2 flags');

SELECT throws_ok(
    'SELECT * FROM pgr_apspWarshall(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table ORDER BY id'',
        FALSE
    )','42883','function pgr_apspwarshall(unknown, boolean) does not exist',
    '15: Documentation says it does needs 2 flags');

SELECT lives_ok(
    'SELECT * FROM pgr_apspWarshall(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table ORDER BY id'',
        FALSE, TRUE
    )',
    '16: Documentation says it does have a Directed & has_rcost flags');


-- Finish the tests and clean up.
SELECT * FROM finish();
ROLLBACK;
