/* --q5 */
SELECT * FROM pgr_drivingDistance(
  'SELECT id, source, target, cost, reverse_cost FROM edge_table',
  11, 3.0);
/* --q6 */
SELECT * FROM pgr_drivingDistance(
  'SELECT id, source, target, cost, reverse_cost FROM edge_table',
  array[11, 16], 3.0, equicost => true);
/* --q10 */
SELECT * FROM pgr_drivingDistance(
  'SELECT id, source, target, cost, reverse_cost FROM edge_table',
  array[11, 16], 3.0, directed => false);
/* --q15 */
