-- CopyRight(c) pgRouting developers
-- Creative Commons Attribution-Share Alike 3.0 License : https://creativecommons.org/licenses/by-sa/3.0/
\set VERBOSITY terse
SET extra_float_digits=-3;

DROP TABLE IF EXISTS old_restrictions;
/* --rest00 */
CREATE TABLE old_restrictions (
    rid BIGINT NOT NULL,
    to_cost FLOAT,
    target_id BIGINT,
    via_path TEXT
);
/* --rest01 */
INSERT INTO old_restrictions (rid, to_cost, target_id, via_path) VALUES
(1, 100,  7,  '4'),
(1, 100, 11,  '8'),
(1, 100, 10,  '7'),
(2,   4,  9,  '5, 3'),
(3, 100,  9, '16');
/* --rest1 */
SELECT * FROM old_restrictions;
/* --rest2 */
SELECT * FROM restrictions;
/* --rest3 */
SELECT rid AS id,
  _pgr_array_reverse(
    array_prepend(target_id, string_to_array(via_path::text, ',')::BIGINT[])) AS path,
  to_cost AS cost
  INTO new_restrictions
FROM old_restrictions;
/* --rest4 */
SELECT * FROM new_restrictions;
/* --rest5 */

/* --verticesv1 */
SELECT * FROM pgr_trsp(
  $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost
    FROM edges WHERE id != 16$$,
  15, 16,
  true, true);
/* --verticesv2 */
SELECT * FROM pgr_dijkstra(
  $$SELECT id, source, target, cost, reverse_cost
    FROM edges WHERE id != 16$$,
  15, 16);
/* --verticesv3 */
SELECT seq, node::INTEGER AS id1, edge::INTEGER AS id2, cost
FROM pgr_dijkstra(
  $$SELECT id, source, target, cost, reverse_cost
    FROM edges WHERE id != 16$$,
  15, 16);
/* --verticesv4 */
SELECT * FROM pgr_trsp(
  $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost
    FROM edges WHERE id != 16$$,
  15, 16,
  true, true,
  $$SELECT to_cost, target_id::INTEGER, via_path
    FROM old_restrictions$$);
/* --verticesv5 */
SELECT * FROM pgr_trsp(
  $$SELECT id, source, target, cost, reverse_cost
    FROM edges WHERE id != 16$$,
  $$SELECT * FROM new_restrictions$$,
  15, 16);
/* --verticesv6 */
SELECT seq, node::INTEGER AS id1, edge::INTEGER AS id2, cost
FROM pgr_trsp(
  $$SELECT id, source, target, cost, reverse_cost
    FROM edges WHERE id != 16$$,
  $$SELECT * FROM new_restrictions$$,
  15, 16);
/* --verticesv7 */
SELECT pid, edge_id, fraction, side FROM pointsOfInterest
WHERE pid IN (3, 4);
/* --edgesv1 */
SELECT * FROM pgr_trsp(
  $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost
    FROM edges$$,
  6, 0.3, 12, 0.6,
  true, true);
/* --edgesv2 */
SELECT * FROM pgr_withPoints(
  $$SELECT id, source, target, cost, reverse_cost FROM edges$$,
  $$SELECT pid, edge_id, fraction FROM pointsOfInterest WHERE pid IN (4, 3)$$,
  -4, -3,
  details => false);
/* --edgesv3 */
SELECT seq, node::INTEGER AS id1, edge::INTEGER AS id2, cost
FROM pgr_withPoints(
  $$SELECT id, source, target, cost, reverse_cost FROM edges$$,
  $$SELECT * FROM (VALUES (1, 6, 0.3),(2, 12, 0.6)) AS t(pid, edge_id, fraction)$$,
  -1, -2,
  details => false);
/* --edgesv4 */
SELECT * FROM pgr_trsp(
  $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edges$$,
  6, 0.3, 12, 0.6, true, true,
  $$SELECT to_cost, target_id::INTEGER, via_path FROM old_restrictions$$);
/* --edgesv5 */
SELECT * FROM pgr_trsp_withPoints(
  $$SELECT id, source, target, cost, reverse_cost FROM edges$$,
  $$SELECT * FROM new_restrictions$$,
  $$SELECT pid, edge_id, fraction FROM pointsOfInterest WHERE pid IN (4, 3)$$,
  -4, -3,
  details => false);
/* --edgesv6 */
SELECT seq, node::INTEGER AS id1, edge::INTEGER AS id2, cost
FROM pgr_trsp_withPoints(
  $$SELECT id, source, target, cost, reverse_cost FROM edges$$,
  $$SELECT * FROM new_restrictions$$,
  $$SELECT * FROM (VALUES (1, 6, 0.3),(2, 12, 0.6)) AS t(pid, edge_id, fraction)$$,
  -1, -2,
  details => false)
WHERE edge != -1;
/* --edgesv7 */
/* --viav1 */
SELECT * FROM pgr_trspViaVertices(
  $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edges$$,
  ARRAY[6, 3, 6],
  true, true);
/* --viav2 */
SELECT * FROM pgr_dijkstraVia(
  $$SELECT id, source, target, cost, reverse_cost FROM edges$$,
  ARRAY[6, 3, 6]);
/* --viav3*/
SELECT row_number() over(ORDER BY seq) AS seq,
  path_id::INTEGER AS id1, node::INTEGER AS id2,
  CASE WHEN edge >= 0 THEN edge::INTEGER ELSE -1 END AS id3, cost::FLOAT
FROM pgr_dijkstraVia(
  $$SELECT id, source, target, cost, reverse_cost FROM edges$$,
  ARRAY[6, 3, 6])
WHERE edge != -1;
/* --viav4*/
SELECT * FROM pgr_trspViaVertices(
  $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edges$$,
  ARRAY[6, 3, 6],
  true, true,
  $$SELECT to_cost, target_id::INTEGER, via_path FROM old_restrictions$$);
/* --viav5 */
SELECT * FROM pgr_trspVia(
  $$SELECT id, source, target, cost, reverse_cost FROM edges$$,
  $$SELECT * FROM new_restrictions$$,
  ARRAY[6, 3, 6]);
/* --viav6*/
SELECT row_number() over(ORDER BY seq) AS seq,
  path_id::INTEGER AS id1, node::INTEGER AS id2,
  CASE WHEN edge >= 0 THEN edge::INTEGER ELSE -1 END AS id3, cost::FLOAT
FROM pgr_trspVia(
  $$SELECT id, source, target, cost, reverse_cost FROM edges$$,
  $$SELECT * FROM new_restrictions$$,
  ARRAY[6, 3, 6])
WHERE edge != -1;
/* --viav7*/
SELECT pid, edge_id, fraction, side FROM pointsOfInterest
WHERE pid IN (3, 4, 6);
/* --edgesvia1 */
SELECT * FROM pgr_trspViaEdges(
  $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edges$$,
  ARRAY[6, 12, 4], ARRAY[0.3, 0.6, 0.7],
  true, true);
/* --edgesvia2 */
SELECT * FROM pgr_withPointsVia(
  $$SELECT id, source, target, cost, reverse_cost FROM edges$$,
  $$SELECT pid, edge_id, fraction FROM pointsOfInterest WHERE pid IN (3, 4, 6)$$,
  ARRAY[-4, -3, -6],
  details => false);
/* --edgesvia3 */
SELECT row_number() over(ORDER BY seq) AS seq,
path_id::INTEGER AS id1, node::INTEGER AS id2,
CASE WHEN edge >= 0 THEN edge::INTEGER ELSE -1 END AS id3, cost::FLOAT
FROM pgr_withPointsVia(
  $$SELECT id, source, target, cost, reverse_cost FROM edges$$,
  $$SELECT * FROM (VALUES (1, 6, 0.3),(2, 12, 0.6),(3, 4, 0.7)) AS t(pid, edge_id, fraction)$$,
  ARRAY[-1, -2, -3],
  details=> false);
/* --edgesvia4 */
SELECT * FROM pgr_trspViaEdges(
  $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edges$$,
  ARRAY[6, 12, 4], ARRAY[0.3, 0.6, 0.7],
  true, true,
  $$SELECT to_cost, target_id::INTEGER, via_path FROM old_restrictions$$);
/* --edgesvia5 */
SELECT * FROM pgr_trspVia_withPoints(
  $$SELECT id, source, target, cost, reverse_cost FROM edges$$,
  $$SELECT * FROM new_restrictions$$,
  $$SELECT pid, edge_id, fraction FROM pointsOfInterest WHERE pid IN (3, 4, 6)$$,
  ARRAY[-4, -3, -6],
  details => false);
/* --edgesvia6 */
SELECT row_number() over(ORDER BY seq) AS seq,
path_id::INTEGER AS id1, node::INTEGER AS id2,
CASE WHEN edge >= 0 THEN edge::INTEGER ELSE -1 END AS id3, cost::FLOAT
FROM pgr_trspVia_withPoints(
  $$SELECT id, source, target, cost, reverse_cost FROM edges$$,
  $$SELECT * FROM new_restrictions$$,
  $$SELECT * FROM (VALUES (1, 6, 0.3),(2, 12, 0.6),(3, 4, 0.7)) AS t(pid, edge_id, fraction)$$,
  ARRAY[-1, -2, -3],
  details => false);
/* --edgesvia7 */

/* --maxcard1 */
SELECT * FROM pgr_maxCardinalityMatch(
  $$SELECT id, source, target, cost AS going, reverse_cost AS coming FROM edges$$,
  directed => true
);
/* --maxcard2 */
SELECT * FROM pgr_maxCardinalityMatch(
  $$SELECT id, source, target, cost, reverse_cost FROM edges$$
);
/* --maxcard3 */
/* --dijkstra1 */
SELECT * FROM pgr_dijkstra(
  $$SELECT id, source, target, cost, reverse_cost FROM edges$$,
  6, 10);
/* --dijkstra2 */
SELECT * FROM pgr_dijkstra(
  $$SELECT id, source, target, cost, reverse_cost FROM edges$$,
  6, ARRAY[3, 10]);
/* --dijkstra3 */
SELECT * FROM pgr_dijkstra(
  $$SELECT id, source, target, cost, reverse_cost FROM edges$$,
  ARRAY[3, 6], 10);
/* --dijkstra4 */
SELECT seq, path_seq, node, edge, cost, agg_cost FROM pgr_dijkstra(
  $$SELECT id, source, target, cost, reverse_cost FROM edges$$,
  6, 10);
/* --dijkstra5 */
