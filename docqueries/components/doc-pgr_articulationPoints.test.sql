/* -- q1 */
SELECT * FROM pgr_articulationPoints(
  'SELECT id, source, target, cost, reverse_cost FROM edge_table'
);
/* -- q2 */
