SELECT * FROM pgr_dijkstraTRSP(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 OR id = 7',
    'SELECT * FROM restrict',
    2, 8
);
