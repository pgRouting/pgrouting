SET extra_float_digits=-3;
/* -- q0 */
SELECT * FROM pgr_withPointsVia(
  'SELECT id, source, target, cost, reverse_cost FROM edge_table order by id',
  'SELECT pid, edge_id, fraction, side from pointsOfInterest',
  ARRAY[-1, -3, 9], false);
/* -- q1 */
SELECT * FROM pgr_withPointsVia(
  'SELECT id, source, target, cost, reverse_cost FROM edge_table order by id',
  'SELECT pid, edge_id, fraction, side from pointsOfInterest',
  ARRAY[-1, 5, -3, 9, 4]);
/* -- q2 */
SELECT agg_cost FROM  pgr_withPointsVia(
  'SELECT id, source, target, cost, reverse_cost FROM edge_table order by id',
  'SELECT pid, edge_id, fraction, side from pointsOfInterest',
  ARRAY[-1, 5, -3, 9, 4])
WHERE path_id = 3 AND edge <0;
/* -- q3 */
SELECT route_agg_cost FROM  pgr_withPointsVia(
  'SELECT id, source, target, cost, reverse_cost FROM edge_table order by id',
  'SELECT pid, edge_id, fraction, side from pointsOfInterest',
  ARRAY[-1, 5, -3, 9, 4])
WHERE path_id = 3 AND edge < 0;
/* -- q4 */
SELECT row_number() over () as node_seq, node
FROM  pgr_withPointsVia(
  'SELECT id, source, target, cost, reverse_cost FROM edge_table order by id',
  'SELECT pid, edge_id, fraction, side from pointsOfInterest',
  ARRAY[-1, 5, -3, 9, 4])
WHERE edge <> -1 ORDER BY seq;
/* -- q5 */
SELECT path_id, route_agg_cost FROM  pgr_withPointsVia(
  'SELECT id, source, target, cost, reverse_cost FROM edge_table order by id',
  'SELECT pid, edge_id, fraction, side from pointsOfInterest',
  ARRAY[-1, 5, -3, 9, 4])
WHERE edge < 0;
/* -- q6 */
SELECT seq, node,
CASE WHEN edge = -1 THEN 'visits'
ELSE 'passes in front'
END as status
FROM  pgr_withPointsVia(
  'SELECT id, source, target, cost, reverse_cost FROM edge_table order by id',
  'SELECT pid, edge_id, fraction, side from pointsOfInterest',
  ARRAY[-1, 5, -3, 9, 4], details => true)
WHERE agg_cost  <> 0 or seq = 1;
/* -- q7 */
