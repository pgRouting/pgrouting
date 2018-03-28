

/*
\echo --q0 Checking for valid contraction
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1',
    ARRAY[-1]::integer[], 1, ARRAY[]::BIGINT[], true);
*/
-- \echo --q0 -------------------------------------------

\echo --q1 Checking dead end contraction for single edge
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1',
    ARRAY[1]::integer[], 1, ARRAY[]::BIGINT[], true);
-- \echo --q1 -------------------------------------------

\echo --q2 Checking dead end contraction for two edges
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id < 3',
    ARRAY[1]::integer[], 1, ARRAY[]::BIGINT[], true);
-- \echo --q2 -------------------------------------------

\echo --q3 Checking dead end contraction for sample data
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    ARRAY[1]::integer[], 1, ARRAY[]::BIGINT[], true);
-- \echo --q3 -------------------------------------------

/*
-- this test fails becuse parameter is wrong
\echo --q4 Checking that forbidden vertices can only be one dimensional or empty
SELECT * FROM pgr_contractGraph(
	'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1',
	ARRAY[ [2,3,4,5], [4,5,6,7] ]::BIGINT[][], ARRAY[0]::integer[], 1, true);
-- \echo --q4 -------------------------------------------
*/

\echo --q5 Checking dead end contraction for a graph with no dead end vertex
SELECT * FROM pgr_contractGraph(
	'SELECT id, source, target, cost, reverse_cost FROM edge_table
	WHERE id = 2 OR id = 4 OR id = 5 OR id = 8',
	ARRAY[1]::integer[], 1, ARRAY[]::BIGINT[], true);
-- \echo --q5 -------------------------------------------

