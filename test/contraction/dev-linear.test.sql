-- GRAPH 1 - 2 <- 3
PREPARE graph_e_1_2 AS
SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id IN (1, 2);

-- GRAPH 6 -> 11 <- 10
PREPARE graph_e_11_12 AS
SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id IN (11, 12);

-- GRAPH 4 -> 3 -> 6 -> 11
PREPARE graph_e_3_5_11 AS
SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id IN (3, 5, 11);

-- GRAPH 6 -> 11 -> 12 - 9 - 6
PREPARE graph_e_9_11_13_15 AS
SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id IN (9, 11, 13, 15);
