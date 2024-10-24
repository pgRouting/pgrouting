-- CopyRight(c) pgRouting developers
-- Creative Commons Attribution-Share Alike 3.0 License : https://creativecommons.org/licenses/by-sa/3.0/
SET extra_float_digits=-3;
/* -- q1 */
SELECT * FROM pgr_withPointsCostMatrix(
  'SELECT id, source, target, cost, reverse_cost FROM edges ORDER BY id',
  'SELECT pid, edge_id, fraction from pointsOfInterest',
  array[-1, 10, 11, -6], directed := false);
/* -- q2 */
SELECT * FROM pgr_TSP(
  $$
  SELECT * FROM pgr_withPointsCostMatrix(
    'SELECT id, source, target, cost, reverse_cost FROM edges ORDER BY id',
    'SELECT pid, edge_id, fraction from pointsOfInterest',
    array[-1, 10, 11, -6], directed := false);
  $$
);
/* -- q3 */
SELECT * FROM pgr_withPointsCostMatrix(
  $e$ SELECT * FROM edges $e$,
  $p$ SELECT edge_id, round(fraction::numeric, 2) AS fraction, side
      FROM pgr_findCloseEdges(
        $$SELECT id, geom FROM edges$$,
        (SELECT ST_POINT(2.9, 1.8)),
        0.5, cap => 2)
  $p$,
  ARRAY[5, 10, -1, -2]);
/* -- q4 */
