/* -- q1 */
SELECT * FROM pgr_sequentialVertexColoring(
    'SELECT id, source, target, cost, reverse_cost FROM edges
    ORDER BY id'
);
/* -- q2 */
