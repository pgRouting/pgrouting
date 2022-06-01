/* -- q1 */
SELECT * FROM pgr_kruskal(
  'SELECT id, source, target, cost, reverse_cost
  FROM edges ORDER BY id'
) ORDER BY edge;
/* -- q2 */
