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

/* -- check1 */
SELECT a.id, b.id
FROM edges AS a, edges AS b
WHERE a.id < b.id AND st_crosses(a.geom, b.geom);
/* -- check2 */
SELECT ST_AsText((ST_Dump(ST_Split(a.geom, b.geom))).geom)
FROM edges AS a, edges AS b
WHERE a.id = 13 AND b.id = 18
UNION
SELECT ST_AsText((ST_Dump(ST_Split(b.geom, a.geom))).geom)
FROM edges AS a, edges AS b
WHERE a.id = 13 AND b.id = 18;
/* -- check3 */
SELECT id FROM vertices
WHERE array_length(in_edges || out_edges, 1) = 1;
/* -- check4 */
