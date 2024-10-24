-- CopyRight(c) pgRouting developers
-- Creative Commons Attribution-Share Alike 3.0 License : https://creativecommons.org/licenses/by-sa/3.0/
SET extra_float_digits=-3;
/* -- q0 */
SELECT * FROM pgr_withPointsVia(
  'SELECT id, source, target, cost, reverse_cost FROM edges order by id',
  'SELECT pid, edge_id, fraction, side from pointsOfInterest',
  ARRAY[-6, 15, -1]);
/* -- q1 */
SELECT * FROM pgr_withPointsVia(
  'SELECT id, source, target, cost, reverse_cost FROM edges order by id',
  'SELECT pid, edge_id, fraction, side from pointsOfInterest',
  ARRAY[-1, 7, -3, 16, 15]);
/* -- q2 */
SELECT agg_cost FROM  pgr_withPointsVia(
  'SELECT id, source, target, cost, reverse_cost FROM edges order by id',
  'SELECT pid, edge_id, fraction, side from pointsOfInterest',
  ARRAY[-1, 7, -3, 16, 15])
WHERE path_id = 3 AND edge < 0;
/* -- q3 */
SELECT route_agg_cost FROM  pgr_withPointsVia(
  'SELECT id, source, target, cost, reverse_cost FROM edges order by id',
  'SELECT pid, edge_id, fraction, side from pointsOfInterest',
  ARRAY[-1, 7, -3, 16, 15])
WHERE path_id = 3 AND edge < 0;
/* -- q4 */
SELECT row_number() over () as node_seq, node
FROM  pgr_withPointsVia(
  'SELECT id, source, target, cost, reverse_cost FROM edges order by id',
  'SELECT pid, edge_id, fraction, side from pointsOfInterest',
  ARRAY[-1, 7, -3, 16, 15])
WHERE edge <> -1 ORDER BY seq;
/* -- q5 */
SELECT path_id, route_agg_cost FROM  pgr_withPointsVia(
  'SELECT id, source, target, cost, reverse_cost FROM edges order by id',
  'SELECT pid, edge_id, fraction, side from pointsOfInterest',
  ARRAY[-1, 7, -3, 16, 15])
WHERE edge < 0;
/* -- q6 */
SELECT seq, node,
CASE WHEN edge = -1 THEN 'visits'
ELSE 'passes in front'
END as status
FROM  pgr_withPointsVia(
  'SELECT id, source, target, cost, reverse_cost FROM edges order by id',
  'SELECT pid, edge_id, fraction, side from pointsOfInterest',
  ARRAY[-1, 7, -3, 16, 15], details => true)
WHERE agg_cost <> 0 or seq = 1;
/* -- q7 */
SELECT * FROM pgr_withPointsVia(
  $e$ SELECT * FROM edges $e$,
  $p$ SELECT edge_id, round(fraction::numeric, 2) AS fraction, side
      FROM pgr_findCloseEdges(
        $$SELECT id, geom FROM edges$$,
        (SELECT ST_POINT(2.9, 1.8)),
        0.5, cap => 2)
  $p$,
  ARRAY[1, -1, -2], details => true);
/* -- q8 */
