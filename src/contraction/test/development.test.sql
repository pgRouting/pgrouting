\echo --q1 Checking for single edge
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1',
    ARRAY[]::BIGINT[], ARRAY[0]::integer[], 1, true);
\echo --q2 Checking for two edges
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id < 3',
    ARRAY[2]::BIGINT[], ARRAY[0]::integer[], 1, true);
\echo --q3 Checking for sample data
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    ARRAY[2]::BIGINT[], ARRAY[0]::integer[], 1, true);
\echo --q4 Checking that forbidden vertices can only be one dimensional or empty
SELECT * FROM pgr_contractGraph(
	'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1',
	ARRAY[ [2,3,4,5], [4,5,6,7] ]::BIGINT[][], ARRAY[0]::integer[], 1, true);