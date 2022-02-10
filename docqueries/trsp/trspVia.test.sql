-- documentation queries
/* -- q0 */
SELECT * FROM pgr_trspVia(
  $$SELECT id, source, target, cost, reverse_cost FROM edge_table order by id$$,
  $$SELECT path, cost FROM restrictions$$,
  ARRAY[1, 7, 10]
);
/* -- q1 */
SELECT * FROM pgr_trspVia(
  $$SELECT id, source, target, cost, reverse_cost FROM edge_table order by id$$,
  $$SELECT path, cost FROM restrictions$$,
  ARRAY[1, 5, 7, 10, 4]
);

/* -- q2 */
SELECT agg_cost FROM  pgr_trspVia(
  $$SELECT id, source, target, cost, reverse_cost FROM edge_table order by id$$,
  $$SELECT path, cost FROM restrictions$$,
  ARRAY[1, 5, 7, 10, 4]
)
WHERE path_id = 3 AND edge <0;


/* -- q3 */
SELECT route_agg_cost FROM  pgr_trspVia(
  $$SELECT id, source, target, cost, reverse_cost FROM edge_table order by id$$,
  $$SELECT path, cost FROM restrictions$$,
  ARRAY[1, 5, 7, 10, 4]
)
WHERE path_id = 3 AND edge < 0;


/* -- q4 */
SELECT row_number() over () as node_seq, node
FROM  pgr_trspVia(
  $$SELECT id, source, target, cost, reverse_cost FROM edge_table order by id$$,
  $$SELECT path, cost FROM restrictions$$,
  ARRAY[1, 5, 7, 10, 4]
)
WHERE edge <> -1 ORDER BY seq;


/* -- q5 */
SELECT path_id, route_agg_cost FROM  pgr_trspVia(
  $$SELECT id, source, target, cost, reverse_cost FROM edge_table order by id$$,
  $$SELECT path, cost FROM restrictions$$,
  ARRAY[1, 5, 7, 10, 4]
)
WHERE edge < 0;


/* -- q6 */
SELECT seq, route_agg_cost, node, agg_cost ,
CASE WHEN edge = -1 THEN $$visits$$
ELSE $$passes in front$$
END as status
FROM  pgr_trspVia(
  $$SELECT id, source, target, cost, reverse_cost FROM edge_table order by id$$,
  $$SELECT path, cost FROM restrictions$$,
  ARRAY[1, 5, 7, 10, 4])
WHERE node = 9 and (agg_cost  <> 0 or seq = 1);
/* -- q7 */
SELECT * FROM pgr_dijkstraVia(
  $$SELECT id, source, target, cost, reverse_cost FROM edge_table order by id$$,
  ARRAY[2, 8, 2]);
/* -- q8 */
SELECT 1 AS path_id, path_seq, start_vid, end_vid, node, edge, cost, agg_cost FROM  pgr_trsp(
  $$SELECT id, source, target, cost, reverse_cost FROM edge_table$$,
  $$SELECT path, cost FROM restrictions$$,
  2,8);
/* -- q9 */
WITH
solutions AS (
  SELECT path_id, path_seq, start_vid, end_vid, node, edge, cost, agg_cost FROM  pgr_dijkstraVia(
    $$SELECT id, source, target, cost, reverse_cost FROM edge_table$$,
    ARRAY[2, 8, 2]) WHERE path_id != 1

  UNION

  SELECT 1 AS path_id, path_seq, start_vid, end_vid, node, edge, cost, agg_cost FROM  pgr_trsp(
    $$SELECT id, source, target, cost, reverse_cost FROM edge_table$$,
    $$SELECT path, cost FROM restrictions$$,
    2,8)),
with_seq AS (
  SELECT row_number() over(ORDER BY path_id, path_seq) AS seq, *
  FROM solutions),
aggregation AS (SELECT seq, SUM(cost) OVER(ORDER BY seq) AS route_agg_cost FROM with_seq)

SELECT with_seq.*, COALESCE(route_agg_cost, 0) AS route_agg_cost
FROM with_seq LEFT JOIN aggregation ON (with_seq.seq = aggregation.seq + 1);
/* -- q10 */
SELECT * FROM  pgr_trspVia(
  $$SELECT id, source, target, cost, reverse_cost FROM edge_table order by id$$,
  $$SELECT path, cost FROM restrictions$$,
  ARRAY[2,8,2]);
/* -- q11 */
