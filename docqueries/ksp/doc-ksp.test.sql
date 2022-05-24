/* --q1 */
SELECT * FROM pgr_KSP(
  'SELECT id, source, target, cost, reverse_cost FROM edge_table',
  6, 17, 2);
/* --q2 */
SELECT * FROM pgr_KSP(
  'SELECT id, source, target, cost, reverse_cost FROM edge_table',
  6, 17, 2,
  directed => false, heap_paths => true
);
/* --q3 */
