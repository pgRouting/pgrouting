/* -- q1 */
SELECT * FROM pgr_transitiveclosure(
  'SELECT id, source, target, cost, reverse_cost
  FROM edge_table WHERE id IN (2, 3, 5, 11, 12, 13, 15)');
/* -- q2 */
