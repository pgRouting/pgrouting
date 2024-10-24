-- CopyRight(c) pgRouting developers
-- Creative Commons Attribution-Share Alike 3.0 License : https://creativecommons.org/licenses/by-sa/3.0/
/* -- q1 */
SELECT  *
FROM pgr_findCloseEdges(
  $$SELECT id, geom FROM edges$$,
  (SELECT geom FROM pointsOfInterest WHERE pid = 5),
  0.5);
/* -- q2 */
SELECT edge_id, round(fraction::numeric, 2) AS fraction, side, ST_AsText(geom) AS original_point
FROM pgr_findCloseEdges(
  $$SELECT id, geom FROM edges$$,
  (SELECT array_agg(geom) FROM pointsOfInterest),
  0.5);
/* -- q3 */
/* One point signature */
/* -- o1 */
SELECT *
FROM pgr_findCloseEdges(
  $$SELECT id, geom FROM edges$$,
  (SELECT geom FROM pointsOfInterest WHERE pid = 5),
  0.5, cap => 2);
/* -- o2 */
SELECT edge_id, round(fraction::numeric, 2) AS fraction, side, round(distance::numeric, 3) AS distance,
  ST_AsText(geom) AS new_point,
  ST_AsText(edge) AS original_to_new_point
FROM pgr_findCloseEdges(
  $$SELECT id, geom FROM edges$$,
  (SELECT geom FROM pointsOfInterest WHERE pid = 5),
  0.5, partial => false);
/* -- o3 */
SELECT edge_id, round(fraction::numeric, 2) AS fraction, side, round(distance::numeric, 3) AS distance,
  ST_AsText(geom) AS new_point,
  ST_AsText(edge) AS original_to_new_point
FROM pgr_findCloseEdges(
  $$SELECT id, geom FROM edges$$,
  (SELECT geom FROM pointsOfInterest WHERE pid = 5),
  0.5, cap => 2, partial => false);
/* -- o4 */
SELECT * FROM pgr_withPoints(
  $e$ SELECT * FROM edges $e$,
  $p$ SELECT edge_id, round(fraction::numeric, 2) AS fraction, side
      FROM pgr_findCloseEdges(
        $$SELECT id, geom FROM edges$$,
        (SELECT geom FROM pointsOfInterest WHERE pid = 5),
        0.5, cap => 2)
  $p$,
  1, ARRAY[-1, -2]);
/* -- o5 */
SELECT *
FROM pgr_findCloseEdges(
  $$SELECT id, geom FROM edges$$,
  (SELECT geom FROM pointsOfInterest WHERE pid = 5),
  0.5,
  dryrun => true);
/* -- o6 */


/* Many point signature */
/* -- m1 */
SELECT edge_id, round(fraction::numeric, 2) AS fraction, side, round(distance::numeric, 3) AS distance,
  ST_AsText(geom) AS geom_is_original, edge
FROM pgr_findCloseEdges(
  $$SELECT id, geom FROM edges$$,
  (SELECT array_agg(geom) FROM pointsOfInterest),
  0.5, cap => 2);
/* -- m2 */
SELECT edge_id, round(fraction::numeric, 2) AS fraction, side, round(distance::numeric, 3) AS distance,
  ST_AsText(geom) AS geom_is_original,
  ST_AsText(edge) AS original_to_new_point
FROM pgr_findCloseEdges(
  $$SELECT id, geom FROM edges$$,
  (SELECT array_agg(geom) FROM pointsOfInterest),
  0.5, partial => false);
/* -- m3 */
SELECT edge_id, round(fraction::numeric, 2) AS fraction, side, round(distance::numeric, 3) AS distance,
  ST_AsText(geom) AS geom_is_original,
  ST_AsText(edge) AS original_to_new_point
FROM pgr_findCloseEdges(
  $$SELECT id, geom FROM edges$$,
  (SELECT array_agg(geom) FROM pointsOfInterest),
  0.5, cap => 2, partial => false);
/* -- m4 */
SELECT *
FROM pgr_findCloseEdges(
  $$SELECT id, geom FROM edges$$,
  (SELECT array_agg(geom) FROM pointsOfInterest),
  0.5,
  dryrun => true);
/* -- m5 */
