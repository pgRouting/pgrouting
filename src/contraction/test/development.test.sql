\echo --q1 Checking for single edge
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1',
    ARRAY[]::BIGINT[], ARRAY[0]::integer[], 1, true);
\echo --q2 Checking for two edges
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id < 3',
    ARRAY[]::BIGINT[], ARRAY[0]::integer[], 1, true);
\echo --q3 Checking for sample data
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    ARRAY[]::BIGINT[], ARRAY[0]::integer[], 1, true);
\echo --q4 Checking that forbidden vertices can only be one dimensional or empty
SELECT * FROM pgr_contractGraph(
	'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1',
	ARRAY[ [2,3,4,5], [4,5,6,7] ]::BIGINT[][], ARRAY[0]::integer[], 1, true);
\echo --q5 Checking contraction for a graph with no dead end vertex
SELECT * FROM pgr_contractGraph(
	'SELECT id, source, target, cost, reverse_cost FROM edge_table 
	WHERE id = 2 OR id = 4 OR id = 5 OR id = 8',
	ARRAY[]::BIGINT[], ARRAY[0]::integer[], 1, true);
\echo --q6 Checking for linear vertices case 1
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table 
    WHERE id = 3 OR id = 5',
    ARRAY[]::BIGINT[], ARRAY[1]::integer[], 1, true);
