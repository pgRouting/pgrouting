SET client_min_messages TO DEBUG;

SELECT * FROM pgr_test_c_edges(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table'
);
