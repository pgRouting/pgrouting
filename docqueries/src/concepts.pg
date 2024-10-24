-- CopyRight(c) pgRouting developers
-- Creative Commons Attribution-Share Alike 3.0 License : https://creativecommons.org/licenses/by-sa/3.0/
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
SELECT id, in_edges, out_edges
FROM pgr_extractVertices('SELECT id, source, target FROM wiki');
/* -- q5 */


/* -- topo1 */
UPDATE edges SET
cost = sign(cost) * ST_length(geom) * 2,
reverse_cost = sign(reverse_cost) * ST_length(geom) * 2;
/* -- topo2 */
SELECT id, cost, reverse_cost FROM edges;
/* -- topo3 */
UPDATE edges SET
cost = sign(cost),
reverse_cost = sign(reverse_cost);
/* -- topo4 */
ALTER TABLE edges ADD COLUMN direction TEXT;
UPDATE edges SET
direction = CASE WHEN (cost>0 AND reverse_cost>0) THEN 'B'   /* both ways */
           WHEN (cost>0 AND reverse_cost<0) THEN 'FT'  /* direction of the LINESSTRING */
           WHEN (cost<0 AND reverse_cost>0) THEN 'TF'  /* reverse direction of the LINESTRING */
           ELSE '' END;                                /* unknown */
/* -- topo5 */
UPDATE edges SET
cost = CASE WHEN (direction = 'B' OR direction = 'FT')
       THEN ST_length(geom) * 2
       ELSE -1 END,
reverse_cost = CASE WHEN (direction = 'B' OR direction = 'TF')
       THEN ST_length(geom) * 2
       ELSE -1 END;
/* -- topo6 */
SELECT id, cost, reverse_cost FROM edges;
/* -- topo7 */
UPDATE edges SET
cost = sign(cost),
reverse_cost = sign(reverse_cost);
ALTER TABLE edges DROP COLUMN direction;
/* -- topo8 */


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



/* -- connect2 */
SELECT * FROM pgr_connectedComponents(
  'SELECT id, source, target, cost, reverse_cost FROM edges'
);
/* -- connect3 */
ALTER TABLE vertices ADD COLUMN component BIGINT;
ALTER TABLE edges ADD COLUMN component BIGINT;
/* -- connect4 */
UPDATE vertices SET component = c.component
FROM (SELECT * FROM pgr_connectedComponents(
  'SELECT id, source, target, cost, reverse_cost FROM edges'
)) AS c
WHERE id = node;
/* -- connect5 */
UPDATE edges SET component = v.component
FROM (SELECT id, component FROM vertices) AS v
WHERE source = v.id;
/* -- connect6 */
SELECT edge_id, fraction, ST_AsText(edge) AS edge, id AS closest_vertex
FROM pgr_findCloseEdges(
  $$SELECT id, geom FROM edges WHERE component = 1$$,
  (SELECT array_agg(geom) FROM vertices WHERE component = 2),
  2, partial => false) JOIN vertices USING (geom) ORDER BY distance LIMIT 1;
/* -- connect7 */
WITH
info AS (
  SELECT
    edge_id, fraction, side, distance, ce.geom, edge, v.id AS closest,
    source, target, capacity, reverse_capacity, e.geom AS e_geom
  FROM pgr_findCloseEdges(
    $$SELECT id, geom FROM edges WHERE component = 1$$,
    (SELECT array_agg(geom) FROM vertices WHERE component = 2),
    2, partial => false) AS ce
  JOIN vertices AS v USING (geom)
  JOIN edges AS e ON (edge_id = e.id)
  ORDER BY distance LIMIT 1),
three_options AS (
  SELECT
    closest AS source, target, 0 AS cost, 0 AS reverse_cost,
    capacity, reverse_capacity,
    ST_X(geom) AS x1, ST_Y(geom) AS y1,
    ST_X(ST_EndPoint(e_geom)) AS x2, ST_Y(ST_EndPoint(e_geom)) AS y2,
    ST_MakeLine(geom, ST_EndPoint(e_geom)) AS geom
  FROM info
  UNION
  SELECT closest, source, 0, 0, capacity, reverse_capacity,
    ST_X(geom) AS x1, ST_Y(geom) AS y1,
    ST_X(ST_StartPoint(e_geom)) AS x2, ST_Y(ST_StartPoint(e_geom)) AS y2,
    ST_MakeLine(info.geom, ST_StartPoint(e_geom))
  FROM info
  /*
  UNION
  -- This option requires splitting the edge
  SELECT closest, NULL, 0, 0, capacity, reverse_capacity,
    ST_X(geom) AS x1, ST_Y(geom) AS y1,
    ST_X(ST_EndPoint(edge)) AS x2, ST_Y(ST_EndPoint(edge)) AS y2,
    edge
  FROM info */
  )
INSERT INTO edges
  (source, target,
    cost, reverse_cost,
    capacity, reverse_capacity,
    x1, y1, x2, y2,
    geom)
(SELECT
    source, target, cost, reverse_cost, capacity, reverse_capacity,
    x1, y1, x2, y2, geom
  FROM three_options);
/* -- connect8 */
SELECT * FROM pgr_connectedComponents(
  'SELECT id, source, target, cost, reverse_cost FROM edges'
);
/* -- connect9 */

/* -- contract1 */
SELECT id FROM vertices
WHERE array_length(in_edges || out_edges, 1) = 1;
/* -- contract2 */
SELECT id FROM vertices
WHERE array_length(in_edges || out_edges, 1) = 2;
/* -- contract3 */

/* -- performance1 */
SELECT * FROM pgr_dijkstra($$
  SELECT id, source, target, cost, reverse_cost from edges
  WHERE geom && (SELECT st_buffer(geom, 1) AS myarea
    FROM edges WHERE id = 2)$$,
  1, 2);
/* -- performance2 */
