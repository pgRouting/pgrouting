/* -- q1 */
SELECT * FROM pgr_contraction(
  'SELECT id, source, target, cost, reverse_cost FROM edges',
  ARRAY[1, 2], directed => false);
/* -- q2 */
SELECT type, id, contracted_vertices FROM pgr_contraction(
  'SELECT id, source, target, cost, reverse_cost FROM edges',
  ARRAY[1]);
/* -- q3 */
SELECT * FROM pgr_contraction(
  'SELECT id, source, target, cost, reverse_cost FROM edges',
  ARRAY[2]);
/* -- q4 */
