
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


\echo --q13 Checking linear contraction for a square like graph
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table
    WHERE id = 2 OR id = 4 OR id = 5 OR id = 8',
    ARRAY[2]::integer[], 1, ARRAY[]::BIGINT[], true);
-- \echo --q13 -------------------------------------------

