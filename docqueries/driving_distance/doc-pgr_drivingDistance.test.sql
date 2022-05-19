
/* --q1 */
SELECT * FROM pgr_drivingDistance(
  'SELECT id, source, target, cost, reverse_cost FROM edge_table',
  6, 3.0);
/* --q2 */
SELECT * FROM pgr_drivingDistance(
  'SELECT id, source, target, cost, reverse_cost FROM edge_table',
  array[6, 9], 3.0, equicost => true);
/* --q3 */
SELECT * FROM pgr_drivingDistance(
  'SELECT id, source, target, cost, reverse_cost FROM edge_table',
  array[6, 9], 3.0, directed => false);
/* --q4 */
