/* -- q1 */
SELECT * FROM pgr_edgeColoring(
    'SELECT id, source, target, cost, reverse_cost FROM edges
    ORDER BY id'
);
/* -- q2 */
