
/* -- q1 */
SELECT * FROM pgr_dagShortestPath(
    'SELECT id, source, target, cost FROM edge_table',
    1, 6
);

/* -- q2 */
SELECT * FROM pgr_dagShortestPath(
    'SELECT id, source, target, cost FROM edge_table',
    1, ARRAY[5,6]
);
/* -- q3 */
SELECT * FROM pgr_dagShortestPath(
    'SELECT id, source, target, cost FROM edge_table',
    ARRAY[1,3], 6
);

/* -- q4 */
SELECT * FROM pgr_dagShortestPath(
    'SELECT id, source, target, cost FROM edge_table',
    ARRAY[1, 4],ARRAY[12,6]
);
/* -- q5 */
SELECT * FROM pgr_dagShortestPath(
    'SELECT id, source, target, cost FROM edge_table',
    'SELECT * FROM ( VALUES (1, 6), (4, 12) ) AS t(source, target)'
);
/* -- q6 */
