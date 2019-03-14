
/*
-- this test fails becuse parameter is wrong
\echo --q0 Checking for valid contraction
ERROR:  Invalid Contraction Type found
HINT:  Contraction type -1 not valid
CONTEXT:  SQL function "pgr_contractgraph" statement 1
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1',
    ARRAY[-1]::integer[], 1, ARRAY[]::BIGINT[], true);
*/
-- \echo --q0 -------------------------------------------

\echo --q1 Checking dead end contraction for single edge
-- GRAPH: 1 <=> 2
-- EXPECTED
--    1 | v    |  2 | {1}                 |     -1 |     -1 |   -1
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1',
    ARRAY[1]::integer[], 1, ARRAY[]::BIGINT[], true);


-- \echo --q1 -------------------------------------------

\echo --q2 Checking dead end contraction for two edges
--GRAPH: 1 <=> 2 <- 3
-- EXPECTED
--   1 | v    |  3 | {1,2}               |     -1 |     -1 |   -1

SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id < 3',
    ARRAY[1]::integer[], 1, ARRAY[]::BIGINT[], true);


-- \echo --q2 -------------------------------------------

\echo --q3 Checking dead end contraction for sample data

-- EXPECTED
--   1 | v    |  2 | {1}                 |     -1 |     -1 |   -1
--   2 | v    |  5 | {7,8}               |     -1 |     -1 |   -1
--   3 | v    | 10 | {13}                |     -1 |     -1 |   -1
--   4 | v    | 15 | {14}                |     -1 |     -1 |   -1
--   5 | v    | 17 | {16}                |     -1 |     -1 |   -1

SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    ARRAY[1]::integer[], 1, ARRAY[]::BIGINT[], true);


-- \echo --q3 -------------------------------------------

\echo --q5 Checking dead end contraction for a graph with no dead end vertex
-- 5 <- 6
-- "    ^
-- 2 <- 3

-- EXPECTED
-- (empty)

SELECT * FROM pgr_contractGraph(
	'SELECT id, source, target, cost, reverse_cost FROM edge_table
	WHERE id = 2 OR id = 4 OR id = 5 OR id = 8',
	ARRAY[1]::integer[], 1, ARRAY[]::BIGINT[], true);

-- \echo --q5 -------------------------------------------
