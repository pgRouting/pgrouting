/* --q1 */
SELECT * FROM pgr_kruskalDFS(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    2
);
/* --q2 */
SELECT * FROM pgr_kruskalDFS(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    ARRAY[13,2], max_depth := 3
);
/* --q3 */
