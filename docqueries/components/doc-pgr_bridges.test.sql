/* -- q1 */
SELECT * FROM pgr_bridges(
  'SELECT id, source, target, cost, reverse_cost FROM edges'
);
/* -- q2 */
