-- CopyRight(c) pgRouting developers
-- Creative Commons Attribution-Share Alike 3.0 License : https://creativecommons.org/licenses/by-sa/3.0/
/* -- q1 */
SELECT * FROM pgr_degree(
  $$SELECT id FROM edges$$,
  $$SELECT id, in_edges, out_edges
    FROM pgr_extractVertices('SELECT id, geom FROM edges')$$);
/* -- q2 */
SELECT * FROM pgr_degree(
  $$SELECT id FROM edges WHERE id < 17$$,
  $$SELECT id, in_edges, out_edges
    FROM pgr_extractVertices('SELECT id, geom FROM edges')$$);
/* -- q3 */
SELECT * FROM pgr_degree(
  $$SELECT id FROM edges WHERE id < 17$$,
  $$SELECT id, in_edges, out_edges
    FROM pgr_extractVertices('SELECT id, geom FROM edges')$$,
  dryrun => true);
/* -- q4 */
