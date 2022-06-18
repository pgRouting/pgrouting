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
-- One point signature
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


-- Many point signature
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

/* -- q5 */

ALTER TABLE vertices ADD COLUMN component BIGINT;

UPDATE vertices SET component = c.component
FROM (SELECT * FROM pgr_connectedComponents(
  'SELECT id, source, target, cost, reverse_cost FROM edges'
)) AS c
WHERE id = node;

ALTER TABLE edges ADD COLUMN component BIGINT;
UPDATE edges SET component = v.component
FROM (SELECT id, component FROM vertices) AS v
WHERE source = v.id;

SELECT *
FROM pgr_findCloseEdges(
  $$SELECT id, geom FROM edges WHERE component = 1$$,
  (SELECT array_agg(geom) FROM vertices WHERE component = 2),
  2, partial => false);
SELECT *
FROM pgr_findCloseEdges(
  $$SELECT id, geom FROM edges WHERE component = 1$$,
  (SELECT array_agg(geom) FROM vertices WHERE component = 2),
  2, partial => false)
ORDER BY Distance
LIMIT 1;
INSERT INTO edges (source, target, cost, reverse_cost, capacity, reverse_capacity, geom)
-- connecting component to ending vertex of edge found
SELECT
  v.id as source, target, 100 AS cost, 100 AS reverse_cost,
  capacity, reverse_capacity,  -- any additional information
  ST_makeLine(fce.geom, ST_endPoint(e.geom))
FROM pgr_findCloseEdges(
  $$SELECT id, geom FROM edges WHERE component = 1$$,
  (SELECT array_agg(geom) FROM vertices WHERE component = 2),
  0.5, partial => false) AS fce
JOIN vertices AS v USING (geom)
JOIN edges e ON (edge_id = e.id);
/* -- q10 */
