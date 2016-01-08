BEGIN;
------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------
--              PGR_kdijkstraPath
------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------
\echo -- q1
SELECT seq, id1 AS source, id2 AS target, cost FROM pgr_kdijkstraCost(
    'SELECT id, source, target, cost FROM edge_table',
    10, array[4,12], false, false);
\echo -- q2
SELECT seq, id1 AS path, id2 AS node, id3 AS edge, cost
  FROM pgr_kdijkstraPath(
      'SELECT id, source, target, cost FROM edge_table',
      10, array[4,12], false, false);
\echo -- q3
SELECT id1 AS path, st_AStext(st_linemerge(st_union(b.the_geom))) AS the_geom
  FROM pgr_kdijkstraPath(
                  'SELECT id, source, target, cost FROM edge_table',
                  10, array[4,12], false, false
            ) a,
            edge_table b
WHERE a.id3=b.id
GROUP by id1
ORDER by id1;
\echo -- q4
ROLLBACK;
