\echo --q0
--Checking dead end contraction with invalid forbidden vertices
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1',
    ARRAY[1]::integer[], 1, ARRAY[20]::BIGINT[], true);

\echo --q1
-- Checking dead end contraction with no dead end node(old test)
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 2 OR id = 4 OR id = 5 OR id = 8',
    ARRAY[1]::integer[], 1, ARRAY[]::BIGINT[], true);

\echo --q2
--Checking dead end contraction for single dead end node(old test)
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1',
    ARRAY[1]::integer[], 1, ARRAY[]::BIGINT[], true);

\echo --q3
-- Checking dead end contraction for two dead end nodes
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 2 or id = 3',
    ARRAY[1]::integer[], 1, ARRAY[]::BIGINT[], true);

\echo --q4
--Checking dead end contraction for multiple dead end nodes
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 OR id = 2 or id = 3',
    ARRAY[1]::integer[], 1, ARRAY[]::BIGINT[], true);

\echo --q5
-- Checking dead end contraction for sample data(old test)
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    ARRAY[1]::integer[], 1, ARRAY[]::BIGINT[], true);

