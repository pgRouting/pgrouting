
\echo -- testing from an existing starting vertex to an non-existing destination in directed graph
\echo -- expecting results: empty set
SELECT * FROM pgr_dijkstraTRSP(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 OR id = 7',
    'SELECT * FROM restrictions',
    2, 3
);

\echo -- testing from an existing starting vertex to an non-existing destination in undirected graph
\echo -- expecting results: empty set
SELECT * FROM pgr_dijkstraTRSP(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 OR id = 7',
    'SELECT * FROM restrictions',
    2, 3,
    FALSE
);

\echo -- testing from an existing starting vertex to an non-existing destination in directed graph
\echo -- expecting results: empty set
SELECT * FROM pgr_dijkstraTRSP(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 OR id = 7',
    'SELECT * FROM restrictions',
    2, 3
);

\echo -- testing from an existing starting vertex to an non-existing destination in undirected graph
\echo -- expecting results: empty set
SELECT * FROM pgr_dijkstraTRSP(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 OR id = 7',
    'SELECT * FROM restrictions',
    2, 3,
    FALSE
);

\echo -- q3
