
\echo --q15 Checking linear contraction for directed base case
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table where id = 2 or id = 3',
    ARRAY[]::BIGINT[], ARRAY[1]::integer[], 1, true);
-- \echo --q15 -------------------------------------------

\echo --q15 Checking linear contraction for undirected base case
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table where id = 2 or id = 3',
    ARRAY[]::BIGINT[], ARRAY[1]::integer[], 1, false);
-- \echo --q15 -------------------------------------------



