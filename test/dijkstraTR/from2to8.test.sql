
SELECT *
FROM pgr_dijkstraTR(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    'SELECT * FROM new_restrictions WHERE id in (1)', 2, 8);

