--  input: 1 <-> 2, forbidden = 20
-- output: 2{1}
--Checking dead end contraction with invalid forbidden vertices
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1',
    ARRAY[1]::integer[], 1, ARRAY[20]::BIGINT[], true);

-- Checking dead end contraction with no dead end node
-- input: 3->2, 2<->5, 5<->6, 3->6  --q1
-- output:
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 2 OR id = 4 OR id = 5 OR id = 8',
    ARRAY[1]::integer[], 1, ARRAY[]::BIGINT[], true);

--   input: 1 <-> 2
-- outputt: 2{1}
--Checking dead end contraction for single dead end node
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1',
    ARRAY[1]::integer[], 1, ARRAY[]::BIGINT[], true);

-- Checking dead end contraction for two dead end nodes
--   input: 2 <- 3 <- 4
-- outputt: 4{2, 3}
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 2 or id = 3',
    ARRAY[1]::integer[], 1, ARRAY[]::BIGINT[], true);

--Checking dead end contraction for multiple dead end nodes
--  input: 1 <-> 2 <- 3 <- 4
--   step: 2{1} <- 3 <- 4
-- output: 4{1, 2, 3}
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 OR id = 2 or id = 3',
    ARRAY[1]::integer[], 1, ARRAY[]::BIGINT[], true);

-- all table
-- 15{14}
-- 16{17}
-- 10{13}
-- 5{7,8}
-- 2{1}
-- Checking dead end contraction for sample data
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    ARRAY[1]::integer[], 1, ARRAY[]::BIGINT[], true);
