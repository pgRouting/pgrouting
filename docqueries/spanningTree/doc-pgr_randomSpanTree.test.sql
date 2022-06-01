/* -- q1 */
SELECT * FROM pgr_randomSpanTree(
  'SELECT id, source, target, cost, reverse_cost FROM edges',
  7);
/* -- q2 */
SELECT * FROM pgr_randomSpanTree(
  'SELECT id, source, target, cost, reverse_cost FROM edges',
  15, false);
/* -- q3 */
