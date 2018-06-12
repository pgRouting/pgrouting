\echo --q0
--Checking linear contraction with invalid forbidden vertices
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 2 or id = 3',
    ARRAY[2]::integer[], 1, ARRAY[20]::BIGINT[], true);

\echo --q1
--Checking linear contraction with no linear vertex
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table
    WHERE id = 1 OR id = 2 OR id = 4',
    ARRAY[2]::integer[], 1, ARRAY[]::BIGINT[], true);

\echo --q2
--Checking linear contraction with one linear vertex(old test)
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table
    WHERE id = 3 OR id = 5',
    ARRAY[2]::integer[], 1, ARRAY[]::BIGINT[], true);

\echo --q3 
--Checking linear contraction with two linear vertices
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table
    WHERE id = 5 OR id = 11 OR id = 13',
    ARRAY[2]::integer[], 1, ARRAY[]::BIGINT[], true);

\echo --q4
--Checking linear contraction with multiple linear vertices
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost FROM edge_table
    WHERE id = ANY(ARRAY[8, 9, 11, 13]) AND cost > 0 
    UNION
    SELECT id, target, source, reverse_cost FROM edge_table
    WHERE id = ANY(ARRAY[15, 16]) AND reverse_cost > 0',
    ARRAY[2]::integer[], 1, ARRAY[]::BIGINT[], true);


