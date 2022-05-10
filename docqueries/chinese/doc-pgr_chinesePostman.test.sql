
/* -- q1 */
SELECT * FROM pgr_chinesePostman(
  'SELECT id, source, target, cost, reverse_cost
  FROM edge_table WHERE id < 17');
/* -- q2 */
SELECT * FROM pgr_chinesePostman(
  'SELECT id, source, target, cost, reverse_cost
  FROM edge_table WHERE id IN (9, 11, 13, 15)');
/* -- q3 */
