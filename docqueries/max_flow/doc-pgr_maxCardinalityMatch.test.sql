/* -- q2 */
SELECT * FROM pgr_maxCardinalityMatch(
  'SELECT id, source, target, cost AS going, reverse_cost AS coming
  FROM edge_table');
/* -- q3 */
