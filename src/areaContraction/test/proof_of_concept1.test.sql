
\echo -- q1
SELECT * FROM pgr_dijkstra(
  'SELECT * FROM edge_table', ARRAY[1,4,7,13],  ARRAY[1,4,7,13]
);

\echo -- q2
