
SELECT *
FROM pgr_dijkstraTR(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    'SELECT * FROM new_restrictions', 2, 8);

