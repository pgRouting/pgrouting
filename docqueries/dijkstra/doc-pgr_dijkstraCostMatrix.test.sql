/* -- q2 */
SELECT * FROM pgr_dijkstraCostMatrix(
  'SELECT id, source, target, cost, reverse_cost FROM edge_table',
  (SELECT array_agg(id) FROM edge_table_vertices_pgr WHERE id < 5),
  false);
/* -- q3 */
SELECT * FROM pgr_TSP(
  $$
  SELECT * FROM pgr_dijkstraCostMatrix(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    (SELECT array_agg(id) FROM edge_table_vertices_pgr WHERE id < 5),
    false)
  $$);
/* -- q4 */
