/* -- q1 */
SELECT * FROM pgr_transitiveclosure(
  'SELECT id, source, target, cost, reverse_cost
  FROM edges WHERE id IN (2, 3, 5, 11, 12, 13, 15)')
ORDER BY vid;
/* -- q2 */
