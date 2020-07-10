\i setup.sql

SELECT plan(1);


-- Check whether the same set of rows are returned always

PREPARE q1 AS
SELECT id, source, target, cost, reverse_cost
FROM edge_table
ORDER BY id;

PREPARE q2 AS
SELECT id, source, target, cost, reverse_cost
FROM edge_table
ORDER BY id DESC;


PREPARE depthFirstSearch1 AS
SELECT *
FROM pgr_depthFirstSearch(
    'q1',
    1
);

PREPARE depthFirstSearch2 AS
SELECT *
FROM pgr_depthFirstSearch(
    'q2',
    1
);

SELECT set_eq('depthFirstSearch1', 'depthFirstSearch2', '1: Should return same set of rows');


SELECT * FROM finish();
ROLLBACK;
