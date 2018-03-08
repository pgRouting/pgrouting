--q0 Checking dead end & linear contraction
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table
    WHERE id = ANY(ARRAY[2, 3, 5, 9, 16])',
    ARRAY[1,2]::integer[], 1, ARRAY[]::BIGINT[], true);
--q0 -------------------------------------------