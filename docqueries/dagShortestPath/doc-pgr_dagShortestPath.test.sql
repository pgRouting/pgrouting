
\echo -- q1
SELECT * FROM pgr_dagShortestPath(
    'SELECT id, source, target, cost FROM edge_table',
    1, 6
);

\echo -- q2
SELECT * FROM pgr_dagShortestPath(
    'SELECT id, source, target, cost FROM edge_table',
    1, ARRAY[5,6]
);
\echo -- q3
SELECT * FROM pgr_dagShortestPath(
    'SELECT id, source, target, cost FROM edge_table',
    ARRAY[1,3], 6
);

\echo -- q4
SELECT * FROM pgr_dagShortestPath(
    'SELECT id, source, target, cost FROM edge_table',
    ARRAY[1, 4],ARRAY[12,6]
);
\echo -- q5
SELECT * FROM pgr_dagShortestPath(
    'SELECT id, source, target, cost FROM edge_table',
    'SELECT * FROM ( VALUES (1, 6), (4, 12) ) AS t(source, target)'
);
\echo -- q6
