
\echo --q11 Checking dead end, linear contraction for sample data
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table' ,
    ARRAY[1, 2]::integer[], 1, ARRAY[]::BIGINT[], true);

\echo --q12 Checking linear contraction, dead end for sample data
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table' ,
     ARRAY[2, 1]::integer[], 1, ARRAY[]::BIGINT[], true);

\echo --q14 Checking linear contraction, dead end for a square like graph
-- GRAPH
-- 5 <- 6
-- "    ^
-- 2 <- 3
-- 1|v|3|{2,5,6}|-1|-1|-1
-- 2|e|-1|{2}|3|5|2
-- 3|e|-2|{2,5}|3|6|3

-- 3 -> 5

SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table
    WHERE id = 2 OR id = 4 OR id = 5 OR id = 8',
    ARRAY[2, 1]::integer[], 1, ARRAY[]::BIGINT[], true);



