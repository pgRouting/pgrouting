BEGIN;
SET client_min_messages TO WARNING;


\echo --q0 Checking for valid contraction
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1',
    ARRAY[-1]::integer[], 1, ARRAY[]::BIGINT[], true);
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

\echo --q6 Checking for linear vertices case 1
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table 
    WHERE id = 3 OR id = 5',
    ARRAY[2]::integer[], 1, ARRAY[]::BIGINT[], true);
-- \echo --q6 -------------------------------------------


\echo --q7 Checking for linear vertices case 2
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table 
    WHERE id = 8 OR id = 10',
    ARRAY[2]::integer[], 1, ARRAY[]::BIGINT[], true);
-- \echo --q7 -------------------------------------------


\echo --q8 Checking for linear vertices case 3
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table 
    WHERE id = 2 OR id = 4',
    ARRAY[2]::integer[], 1, ARRAY[]::BIGINT[], true);
-- \echo --q8 -------------------------------------------

\echo --q9 Checking for linear vertices case 4
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table 
    WHERE id = 5 OR id = 9',
    ARRAY[2]::integer[], 1, ARRAY[]::BIGINT[], true);
-- \echo --q9 -------------------------------------------

\echo --q10 Checking linear contraction for sample data
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table' ,
    ARRAY[2]::integer[], 1, ARRAY[]::BIGINT[], true);
-- \echo --q10 -------------------------------------------

\echo --q11 Checking dead end, linear contraction for sample data
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table' ,
    ARRAY[1, 2]::integer[], 1, ARRAY[]::BIGINT[], true);
-- \echo --q11 -------------------------------------------

\echo --q12 Checking linear contraction, dead end for sample data
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table' ,
     ARRAY[2, 1]::integer[], 1, ARRAY[]::BIGINT[], true);
-- \echo --q12 -------------------------------------------
\echo --q13 Checking linear contraction for a square like graph
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table 
    WHERE id = 2 OR id = 4 OR id = 5 OR id = 8',
    ARRAY[2]::integer[], 1, ARRAY[]::BIGINT[], true);
-- \echo --q13 -------------------------------------------

\echo --q14 Checking linear contraction, dead end for a square like graph
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table 
    WHERE id = 2 OR id = 4 OR id = 5 OR id = 8',
    ARRAY[2, 1]::integer[], 1, ARRAY[]::BIGINT[], true);



ROLLBACK;
