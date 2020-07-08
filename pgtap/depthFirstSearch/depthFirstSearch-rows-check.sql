\i setup.sql

SELECT plan(5);


-- Check whether the same set of rows are returned always

PREPARE q1 AS
SELECT id, source, target, cost, reverse_cost
FROM edge_table
ORDER BY id;

PREPARE q2 AS
SELECT id, source, target, cost, reverse_cost
FROM edge_table
ORDER BY id DESC;

SELECT SETSEED(1);

PREPARE q3 AS
SELECT id, source, target, cost, reverse_cost
FROM edge_table
ORDER BY RANDOM();

PREPARE q4 AS
SELECT id, source, target, cost, reverse_cost
FROM edge_table
ORDER BY RANDOM();

PREPARE q5 AS
SELECT id, source, target, cost, reverse_cost
FROM edge_table
ORDER BY RANDOM();

PREPARE q6 AS
SELECT id, source, target, cost, reverse_cost
FROM edge_table
ORDER BY RANDOM();

SELECT set_eq('q1', 'q2', '1: Should return same set of rows');
SELECT set_eq('q1', 'q3', '2: Should return same set of rows');
SELECT set_eq('q1', 'q4', '3: Should return same set of rows');
SELECT set_eq('q1', 'q5', '4: Should return same set of rows');
SELECT set_eq('q1', 'q6', '5: Should return same set of rows');


SELECT * FROM finish();
ROLLBACK;
