/* -- q2 */
SELECT * FROM pgr_maxCardinalityMatch(
  'SELECT id, source, target, cost, reverse_cost FROM edges');
/* -- q3 */
