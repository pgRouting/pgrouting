/* -- q1 */
SELECT * FROM pgr_johnson(
    'SELECT source, target, cost FROM edge_table WHERE id < 5'
);
/* -- q2 */
