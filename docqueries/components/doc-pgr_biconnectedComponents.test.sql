/* -- q1 */
SELECT * FROM pgr_biconnectedComponents(
  'SELECT id, source, target, cost, reverse_cost FROM edges'
);
/* -- q2 */
