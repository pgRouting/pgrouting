\echo --q1
SELECT  * FROM pgr_extractVertices(
    'SELECT id, the_geom AS geom
    FROM edge_table');
\echo --q1.1

\echo --q2
SELECT  * FROM pgr_extractVertices(
    'SELECT id, the_geom AS geom FROM edge_table',
    dryrun := true);
\echo --q2.1

\echo --q3
DROP TABLE IF EXISTS vertices_table;
\echo --q3.1

UPDATE edge_table
SET source = NULL, target = NULL,
   x1 = NULL, y1 = NULL,
   x2 = NULL, y2 = NULL;
\echo --q3.2

SELECT  * INTO vertices_table
FROM pgr_extractVertices('SELECT id, the_geom AS geom FROM edge_table');
\echo --q3.3

SELECT *
FROM vertices_table;
\echo --q3.4

WITH
    out_going AS (
        SELECT id AS vid, unnest(out_edges) AS eid, x, y
        FROM vertices_table
    )
UPDATE edge_table
SET source = vid, x1 = x, y1 = y
FROM out_going WHERE id = eid;
\echo --q3.5

WITH
    in_coming AS (
        SELECT id AS vid, unnest(in_edges) AS eid, x, y
        FROM vertices_table
    )
UPDATE edge_table
SET target = vid, x2 = x, y2 = y
FROM in_coming WHERE id = eid;

\echo --q3.6
SELECT id, source, target, x1, y1, x2, y2
FROM edge_table;
\echo --q3.7

