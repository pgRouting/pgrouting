/* -- q1 */
SELECT * FROM pgr_primDD(
  'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
  6, 3.5);
/* -- q2 */
SELECT * FROM pgr_primDD(
  'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
  ARRAY[9, 6], 3.5);
/* -- q3 */
