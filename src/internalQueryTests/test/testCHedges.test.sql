SELECT * FROM pgr_test_c_edges(
    'SELECT id, source, target, cost, reverse_cost, x1, y1, x2, y2 FROM edge_table'
);