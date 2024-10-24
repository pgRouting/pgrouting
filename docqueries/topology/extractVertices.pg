-- CopyRight(c) pgRouting developers
-- Creative Commons Attribution-Share Alike 3.0 License : https://creativecommons.org/licenses/by-sa/3.0/
/* --q1 */
SELECT  * FROM pgr_extractVertices(
  'SELECT id, geom FROM edges');
/* --q1.1 */
/* --q2 */
SELECT  * FROM pgr_extractVertices(
  'SELECT id, geom FROM edges',
  dryrun => true);
/* --q2.1 */
/* --q3 */
DROP TABLE IF EXISTS vertices_table;
/* --q3.1 */
UPDATE edges
SET source = NULL, target = NULL,
x1 = NULL, y1 = NULL,
x2 = NULL, y2 = NULL;
/* --q3.2 */
SELECT  * INTO vertices_table
FROM pgr_extractVertices('SELECT id, geom FROM edges ORDER BY id');
/* --q3.3 */
SELECT *
FROM vertices_table;
/* --q3.4 */
WITH
out_going AS (
  SELECT id AS vid, unnest(out_edges) AS eid, x, y
  FROM vertices_table
)
UPDATE edges
SET source = vid, x1 = x, y1 = y
FROM out_going WHERE id = eid;
/* --q3.5 */
WITH
in_coming AS (
  SELECT id AS vid, unnest(in_edges) AS eid, x, y
  FROM vertices_table
)
UPDATE edges
SET target = vid, x2 = x, y2 = y
FROM in_coming WHERE id = eid;
/* --q3.6 */
SELECT id, source, target, x1, y1, x2, y2
FROM edges ORDER BY id;
/* --q3.7 */
