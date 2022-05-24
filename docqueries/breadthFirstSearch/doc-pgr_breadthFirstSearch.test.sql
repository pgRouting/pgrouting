/* -- q1 */
SELECT * FROM pgr_breadthFirstSearch(
  'SELECT id, source, target, cost, reverse_cost
  FROM edge_table ORDER BY id',
  6);
/* -- q2 */
SELECT * FROM pgr_breadthFirstSearch(
  'SELECT id, source, target, cost, reverse_cost
  FROM edge_table ORDER BY id',
  ARRAY[12, 6], directed => false, max_depth => 2);
/* -- q3 */
SELECT * FROM pgr_breadthFirstSearch(
  'SELECT id, source, target, cost, reverse_cost
  FROM edge_table ORDER BY id DESC',
  6);
/* -- q4 */
