--q0 Checking dead end contraction with no dead end node
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 2 OR id = 4 OR id = 5 OR id = 8',
    ARRAY[1]::integer[], 1, ARRAY[]::BIGINT[], true);
--q0 -------------------------------------------

--q1 Checking dead end contraction for single dead end node
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1',
    ARRAY[1]::integer[], 1, ARRAY[]::BIGINT[], true);
--q1 -------------------------------------------

--q2 Checking dead end contraction for two dead end nodes
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 2 or id = 3',
    ARRAY[1]::integer[], 1, ARRAY[]::BIGINT[], true);
--q2 -------------------------------------------

--q3 Checking dead end contraction for multiple dead end nodes
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 OR id = 2 or id = 3',
    ARRAY[1]::integer[], 1, ARRAY[]::BIGINT[], true);
--q3 -------------------------------------------

--q4 Checking dead end contraction with forbidden vertices not belonging to input graph
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 2 or id = 3',
    ARRAY[1]::integer[], 1, ARRAY[7,8]::BIGINT[], true);
--q4 -------------------------------------------

