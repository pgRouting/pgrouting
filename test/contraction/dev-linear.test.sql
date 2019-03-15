
PREPARE graph_e_1_2 AS
    SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id IN (1, 2);

-- TWO EDGES

-- GRAPH 1 - 2 - 3
-- no forbidden vertices
SELECT * FROM pgr_contractgraph(
    'graph_e_1_2',
    ARRAY[2]::integer[], 1, ARRAY[]::INTEGER[], false);
