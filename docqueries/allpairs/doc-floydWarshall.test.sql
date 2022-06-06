/* -- q1 */
SELECT * FROM pgr_floydWarshall(
    'SELECT id, source, target, cost, reverse_cost FROM edges where id < 5'
);
/* -- q2 */
