/* -- g1 */
SELECT *
FROM (VALUES (1, 1, 2, 5), (2, 1, 3, -3))
     AS t(id, source, target, cost);
/* -- g2 */
SELECT *
FROM (VALUES (1, 1, 2, 5, 2), (2, 1, 3, -3, 4), (3, 2, 3, 7, -1))
     AS t(id, source, target, cost, reverse_cost);
/* -- g3 */




DROP TABLE IF EXISTS wiki;
/* -- q1 */
CREATE TABLE wiki (
  id SERIAL,
  source INTEGER,
  target INTEGER,
  cost INTEGER);
/* -- q2 */
INSERT INTO wiki (source, target, cost) VALUES
(1, 2, 7),  (1, 3, 9), (1, 6, 14),
(2, 3, 10), (2, 4, 15),
(3, 6, 2),  (3, 4, 11),
(4, 5, 6),
(5, 6, 9);
/* -- q3 */
SELECT * FROM pgr_dijkstra(
  'SELECT id, source, target, cost FROM wiki',
  1, 5, false);
/* -- q4 */
SELECT id, source, target FROM wiki;
SELECT id, in_edges, out_edges
FROM pgr_extractVertices('SELECT id, source, target FROM wiki');
/* -- q5 */


/* -- topo1 */
UPDATE edges SET
cost = sign(cost) * ST_length(geom),
reverse_cost = sign(reverse_cost) * ST_length(geom);
/* -- topo2 */
SELECT id, cost, reverse_cost FROM edges;
/* -- topo3 */
UPDATE edges SET
cost = sign(cost),
reverse_cost = sign(reverse_cost);
/* -- topo4 */

/* -- cross1 */
SELECT a.id, b.id
FROM edges AS a, edges AS b
WHERE a.id < b.id AND st_crosses(a.geom, b.geom);
/* -- cross2 */
SELECT ST_AsText((ST_Dump(ST_Split(a.geom, b.geom))).geom)
FROM edges AS a, edges AS b
WHERE a.id = 13 AND b.id = 18
UNION
SELECT ST_AsText((ST_Dump(ST_Split(b.geom, a.geom))).geom)
FROM edges AS a, edges AS b
WHERE a.id = 13 AND b.id = 18;
/* -- cross3 */
WITH
first_edge AS (
  SELECT (ST_Dump(ST_Split(a.geom, b.geom))).path[1],
    (ST_Dump(ST_Split(a.geom, b.geom))).geom,
    ST_LineLocatePoint(a.geom,ST_Intersection(a.geom,b.geom)) AS factor
  FROM edges AS a, edges AS b
  WHERE a.id = 13 AND b.id = 18),
first_segments AS (
  SELECT path, first_edge.geom,
    capacity, reverse_capacity,
    CASE WHEN path=1 THEN factor * cost
         ELSE (1 - factor) * cost END AS cost,
    CASE WHEN path=1 THEN factor * reverse_cost
         ELSE (1 - factor) * reverse_cost END AS reverse_cost
  FROM first_edge , edges WHERE id = 13),

second_edge AS (
  SELECT (ST_Dump(ST_Split(b.geom, a.geom))).path[1],
    (ST_Dump(ST_Split(b.geom, a.geom))).geom,
    ST_LineLocatePoint(b.geom,ST_Intersection(a.geom,b.geom)) AS factor
  FROM edges AS a, edges AS b
  WHERE a.id = 13 AND b.id = 18),
second_segments AS (
  SELECT path, second_edge.geom,
    capacity, reverse_capacity,
    CASE WHEN path=1 THEN factor * cost
         ELSE (1 - factor) * cost END AS cost,
    CASE WHEN path=1 THEN factor * reverse_cost
         ELSE (1 - factor) * reverse_cost END AS reverse_cost
  FROM second_edge , edges WHERE id = 18),
all_segments AS (
  SELECT * FROM first_segments
  UNION
  SELECT * FROM second_segments)

INSERT INTO edges
  (capacity, reverse_capacity,
    cost, reverse_cost,
    x1, y1, x2, y2,
    geom)
(SELECT capacity, reverse_capacity, cost, reverse_cost,
  ST_X(ST_StartPoint(geom)), ST_Y(ST_StartPoint(geom)),
  ST_X(ST_EndPoint(geom)), ST_Y(ST_EndPoint(geom)),
  geom
  FROM all_segments);
/* -- cross4 */
INSERT INTO vertices (in_edges, out_edges, x, y, geom)
(SELECT nv.in_edges, nv.out_edges, nv.x, nv.y, nv.geom
FROM pgr_extractVertices('SELECT id, geom FROM edges') AS nv
LEFT JOIN vertices AS v USING(geom) WHERE v.geom IS NULL);
/* -- cross5 */
/* -- set the source information */
UPDATE edges AS e
SET source = v.id
FROM vertices AS v
WHERE source IS NULL AND ST_StartPoint(e.geom) = v.geom;
/* -- set the target information */
UPDATE edges AS e
SET target = v.id
FROM vertices AS v
WHERE target IS NULL AND ST_EndPoint(e.geom) = v.geom;
/* -- cross6 */
DELETE FROM edges WHERE id IN (13, 18);
/* -- cross7 */
UPDATE vertices AS v SET
in_edges = nv.in_edges, out_edges = nv.out_edges
FROM (SELECT * FROM pgr_extractVertices('SELECT id, geom FROM edges')) AS nv
WHERE v.geom = nv.geom;
/* -- cross8 */
SELECT a.id, b.id
FROM edges AS a, edges AS b
WHERE a.id < b.id AND st_crosses(a.geom, b.geom);
/* -- cross9 */







/* -- deadend1 */
SELECT id FROM vertices
WHERE array_length(in_edges || out_edges, 1) = 1;
/* -- deadend2 */
