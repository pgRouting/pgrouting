-- GRAPH 4 -> 3 -> 6 -> 11
PREPARE graph_e_3_5_11 AS
    SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id IN (3, 5, 11);

-- TWO EDGES

-- no forbidden vertices
SELECT * FROM pgr_contractgraph(
    'graph_e_3_5_11',
    ARRAY[2]::integer[], 1, ARRAY[]::INTEGER[], true);
