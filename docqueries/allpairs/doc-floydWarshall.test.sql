/* -- q1 */
SELECT * FROM pgr_floydWarshall(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table where id < 5'
);
/* -- q2 */
