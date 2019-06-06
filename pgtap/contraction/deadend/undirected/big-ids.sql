\i setup.sql

SELECT plan(6);

UPDATE edge_table
SET id = id + POWER(10, 8),
    source = source + POWER(10, 8),
    target = target + POWER(10, 8);


--  input: 1 - 2, forbidden = 20
-- output: 2{1}
--Checking dead end contraction with invalid forbidden vertices
PREPARE q1 AS
SELECT * FROM pgr_contraction(
    $$SELECT id, source, target, cost, reverse_cost FROM edge_table
    WHERE id = 100000001$$,
    ARRAY[1]::integer[], 1, ARRAY[20]::BIGINT[], false);

SELECT set_eq('q1',
    $$SELECT
    'v'::CHAR AS type,
    100000002::BIGINT AS id,
    ARRAY[100000001]::BIGINT[] AS contracted_vertices,
    -1::BIGINT AS source,
    -1::BIGINT AS target,
    -1::FLOAT AS cost$$);

-- Checking dead end contraction with no dead end node
-- input: 3 - 2 - 5 - 6 - 3  --q1
-- output:
PREPARE q2 AS
SELECT * FROM pgr_contraction(
    $$SELECT id, source, target, cost, reverse_cost FROM edge_table
    WHERE id IN (100000002, 100000004, 100000005, 100000008)$$,
    ARRAY[1]::integer[], 1, ARRAY[]::BIGINT[], false);

SELECT is_empty('q2');

--   input: 1 - 2
-- outputt: 2{1}
--Checking dead end contraction for single dead end node
PREPARE q3 AS
SELECT * FROM pgr_contraction(
    $$SELECT id, source, target, cost, reverse_cost FROM edge_table
    WHERE id = 100000001$$,
    ARRAY[1]::integer[], 1, ARRAY[]::BIGINT[], false);

SELECT set_eq('q3',
    $$SELECT
    'v'::CHAR AS type,
    100000002::BIGINT AS id,
    ARRAY[100000001]::BIGINT[] AS contracted_vertices,
    -1::BIGINT AS source,
    -1::BIGINT AS target,
    -1::FLOAT AS cost$$);

-- Checking dead end contraction for two dead end nodes
--  input: 2 - 3 - 4
-- output: 4{2, 3}
PREPARE q4 AS
SELECT * FROM pgr_contraction(
    $$SELECT id, source, target, cost, reverse_cost FROM edge_table
    WHERE id IN (100000002, 100000003)$$,
    ARRAY[1]::integer[], 1, ARRAY[]::BIGINT[], false);

SELECT set_eq('q4',
    $$SELECT
    'v'::CHAR AS type,
    100000004::BIGINT AS id,
    ARRAY[100000002,100000003]::BIGINT[] AS contracted_vertices,
    -1::BIGINT AS source,
    -1::BIGINT AS target,
    -1::FLOAT AS cost$$);

--Checking dead end contraction for multiple dead end nodes
--  input: 1 - 2 - 3 - 4
-- output: 4{1, 2, 3}
PREPARE q5 AS
SELECT * FROM pgr_contraction(
    $$SELECT id, source, target, cost, reverse_cost FROM edge_table
    WHERE id IN (100000001, 100000002, 100000003)$$,
    ARRAY[1]::integer[], 1, ARRAY[]::BIGINT[], false);

PREPARE sol5 AS
SELECT type, id, contracted_vertices, source, target, cost
FROM (VALUES
    ('v'::CHAR, 100000004::BIGINT, ARRAY[100000001, 100000002, 100000003]::BIGINT[], -1::BIGINT, -1::BIGINT, -1::FLOAT)
) AS t(type, id, contracted_vertices, source, target, cost );

SELECT set_eq('q5', 'sol5');

-- all table
-- 15{14}
-- 16{17}
-- 10{13}
-- 5{7,8}
-- 2{1}
-- Checking dead end contraction for sample data
PREPARE q6 AS
SELECT * FROM pgr_contraction(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    ARRAY[1]::integer[], 1, ARRAY[]::BIGINT[], false);

PREPARE sol6 AS
SELECT type, id, contracted_vertices, source, target, cost
FROM (VALUES
    ('v'::CHAR, 100000002::BIGINT, ARRAY[100000001]::BIGINT[], -1::BIGINT, -1::BIGINT, -1::FLOAT),
    ('v', 100000005, ARRAY[100000007,100000008], -1, -1, -1),
    ('v', 100000010, ARRAY[100000013], -1, -1, -1),
    ('v', 100000015, ARRAY[100000014], -1, -1, -1),
    ('v', 100000017, ARRAY[100000016], -1, -1, -1)
) AS t(type, id, contracted_vertices, source, target, cost );

SELECT set_eq('q6', 'sol6');

SELECT finish();
