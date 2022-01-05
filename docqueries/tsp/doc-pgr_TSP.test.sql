SET extra_float_digits=-3;
SET client_min_messages TO WARNING;

/* -- q1 */
SELECT * FROM pgr_TSP(
  $$
  SELECT * FROM pgr_dijkstraCostMatrix(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    (SELECT array_agg(id) FROM edge_table_vertices_pgr WHERE id < 14),
    directed => false)
  $$);
/* -- q2 */
SELECT * FROM pgr_TSP(
  $$
  SELECT * FROM pgr_dijkstraCostMatrix(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    (SELECT array_agg(id) FROM edge_table_vertices_pgr WHERE id < 14),
    directed => false
  )
  $$,
  start_id => 7
);
/* -- q3 */
SELECT * FROM pgr_TSP(
  $$
  SELECT * FROM pgr_withPointsCostMatrix(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    'SELECT pid, edge_id, fraction from pointsOfInterest',
    array[-1, 3, 5, 6, -6],
    directed => true)
  $$,
  start_id => 5,
  end_id => 6
);
/* -- q4 */
SELECT source AS start_vid, target AS end_vid, 1 AS agg_cost
FROM edge_table WHERE id IN (2, 4, 5, 8, 9, 15);
/* -- q5 */
SELECT * FROM pgr_TSP(
  $$
  SELECT source AS start_vid, target AS end_vid, 1 AS agg_cost
  FROM edge_table WHERE id IN (2, 4, 5, 8, 9, 15)
  $$);
/* -- q6 */
