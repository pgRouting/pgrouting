/* -- q1 */
SELECT * FROM pgr_primDFS(
  'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
  6);
/* -- q2 */
SELECT * FROM pgr_primDFS(
  'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
  ARRAY[9, 6], max_depth => 3);
/* -- q3 */
