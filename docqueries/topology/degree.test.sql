/* -- q1 */
SELECT * FROM pgr_degree(
  $$SELECT id FROM edge_table$$,
  $$SELECT id, in_edges, out_edges
    FROM pgr_extractVertices('SELECT id, the_geom AS geom FROM edge_table')$$);
/* -- q2 */
SELECT * FROM pgr_degree(
  $$SELECT id FROM edge_table WHERE id < 17$$,
  $$SELECT id, in_edges, out_edges
    FROM pgr_extractVertices('SELECT id, the_geom AS geom FROM edge_table')$$);
/* -- q3 */
SELECT * FROM pgr_degree(
  $$SELECT id FROM edge_table WHERE id < 17$$,
  $$SELECT id, in_edges, out_edges
    FROM pgr_extractVertices('SELECT id, the_geom AS geom FROM edge_table')$$,
  dryrun => true);
/* -- q4 */
