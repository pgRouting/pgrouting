\i setup.sql
    SELECT plan(1);

    UPDATE edge_table SET cost = cost + 0.001 * id * id, reverse_cost = reverse_cost + 0.001 * id * id;

    prepare q1 AS
    SELECT seq, id1, id2, cost::TEXT FROM pgr_trsp(
        'select id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost from edge_table',
        1,     -- node_id of start
        5,     -- node_id of end
        true,  -- directed graph?
        true,  -- has_reverse_cost?
        null); -- no turn restrictions

    prepare q2 AS
    SELECT seq-1, node::INTEGER, edge::INTEGER, cost::TEXT FROM pgr_dijkstra(
        'select id, source, target, cost, reverse_cost from edge_table',
        1, 5);

    SELECT set_eq('q2', 'q1', 'No turn restriction from 1 to 5 returns same as dijkstra');

    -- Finish the tests and clean up.
    SELECT * FROM finish();
    ROLLBACK;
