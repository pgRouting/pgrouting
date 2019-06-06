\i setup.sql

SELECT plan(6);

UPDATE edge_table
SET id = id + POWER(10, 12),
    source = source + POWER(10, 12),
    target = target + POWER(10, 12);


--  input: 1 <-> 2, forbidden = 20
-- output: 2{1}
--Checking dead end contraction with invalid forbidden vertices
PREPARE q1 AS
SELECT * FROM pgr_contraction(
    $$SELECT id, source, target, cost, reverse_cost FROM edge_table
    WHERE id = 1000000000001$$,
    ARRAY[1]::integer[], 1, ARRAY[20]::BIGINT[], true);

SELECT set_eq('q1',
    $$SELECT
    'v'::CHAR AS type,
    1000000000002::BIGINT AS id,
    ARRAY[1000000000001]::BIGINT[] AS contracted_vertices,
    -1::BIGINT AS source,
    -1::BIGINT AS target,
    -1::FLOAT AS cost$$);

-- Checking dead end contraction with no dead end node
-- input: 3->2, 2<->5, 5<->6, 3->6  --q1
-- output:
PREPARE q2 AS
SELECT * FROM pgr_contraction(
    $$SELECT id, source, target, cost, reverse_cost FROM edge_table
    WHERE id IN (1000000000002, 1000000000004, 1000000000005, 1000000000008)$$,
    ARRAY[1]::integer[], 1, ARRAY[]::BIGINT[], true);

SELECT is_empty('q2');

--   input: 1 <-> 2
-- outputt: 2{1}
--Checking dead end contraction for single dead end node
PREPARE q3 AS
SELECT * FROM pgr_contraction(
    $$SELECT id, source, target, cost, reverse_cost FROM edge_table
    WHERE id = 1000000000001$$,
    ARRAY[1]::integer[], 1, ARRAY[]::BIGINT[], true);

SELECT set_eq('q3',
    $$SELECT
    'v'::CHAR AS type,
    1000000000002::BIGINT AS id,
    ARRAY[1000000000001]::BIGINT[] AS contracted_vertices,
    -1::BIGINT AS source,
    -1::BIGINT AS target,
    -1::FLOAT AS cost$$);

-- Checking dead end contraction for two dead end nodes
--  input: 2 <- 3 <- 4
-- output: 4{2, 3}
PREPARE q4 AS
SELECT * FROM pgr_contraction(
    $$SELECT id, source, target, cost, reverse_cost FROM edge_table
    WHERE id IN (1000000000002, 1000000000003)$$,
    ARRAY[1]::integer[], 1, ARRAY[]::BIGINT[], true);

SELECT set_eq('q4',
    $$SELECT
    'v'::CHAR AS type,
    1000000000004::BIGINT AS id,
    ARRAY[1000000000002,1000000000003]::BIGINT[] AS contracted_vertices,
    -1::BIGINT AS source,
    -1::BIGINT AS target,
    -1::FLOAT AS cost$$);

--Checking dead end contraction for multiple dead end nodes
--  input: 1 <-> 2 <- 3 <- 4
--   step: 2{1} <- 3 <- 4
-- output: 4{1, 2, 3}
PREPARE q5 AS
SELECT * FROM pgr_contraction(
    $$SELECT id, source, target, cost, reverse_cost FROM edge_table
    WHERE id IN (1000000000001, 1000000000002, 1000000000003)$$,
    ARRAY[1]::integer[], 1, ARRAY[]::BIGINT[], true);

PREPARE sol5 AS
SELECT type, id, contracted_vertices, source, target, cost
FROM (VALUES
    ('v'::CHAR, 1000000000004::BIGINT, ARRAY[1000000000001, 1000000000002, 1000000000003]::BIGINT[], -1::BIGINT, -1::BIGINT, -1::FLOAT)
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
    ARRAY[1]::integer[], 1, ARRAY[]::BIGINT[], true);

PREPARE sol6 AS
SELECT type, id, contracted_vertices, source, target, cost
FROM (VALUES
    ('v'::CHAR, 1000000000002::BIGINT, ARRAY[1000000000001]::BIGINT[], -1::BIGINT, -1::BIGINT, -1::FLOAT),
    ('v', 1000000000005, ARRAY[1000000000007,1000000000008], -1, -1, -1),
    ('v', 1000000000010, ARRAY[1000000000013], -1, -1, -1),
    ('v', 1000000000015, ARRAY[1000000000014], -1, -1, -1),
    ('v', 1000000000017, ARRAY[1000000000016], -1, -1, -1)
) AS t(type, id, contracted_vertices, source, target, cost );

SELECT set_eq('q6', 'sol6');

SELECT finish();
