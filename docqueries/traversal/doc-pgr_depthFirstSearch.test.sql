/* -- q1 */
SELECT * FROM pgr_depthFirstSearch(
  'SELECT id, source, target, cost, reverse_cost FROM edge_table
  ORDER BY id',
  2);
/* -- q2 */
SELECT * FROM pgr_depthFirstSearch(
  'SELECT id, source, target, cost, reverse_cost FROM edge_table
  ORDER BY id',
  ARRAY[11, 2], directed => false, max_depth => 2);
/* -- q3 */
SELECT * FROM pgr_depthFirstSearch(
  'SELECT id, source, target, cost, reverse_cost FROM edge_table
  ORDER BY id DESC',
  2);
/* -- q4 */
