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
