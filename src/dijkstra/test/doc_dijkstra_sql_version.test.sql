
\echo -- q1
SELECT * from pgr_dijkstra(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    1,
    'SELECT id from edge_table_vertices_pgr where id > 3 and id < 6');

\echo -- q2

SELECT * from pgr_dijkstra(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    'SELECT id from edge_table_vertices_pgr where id > 3 and id < 6',
    1);

\echo -- q3

SELECT * from pgr_dijkstra(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    'SELECT id from edge_table_vertices_pgr where id < 3',
    'SELECT id from edge_table_vertices_pgr where id > 3 and id < 6'
    );
