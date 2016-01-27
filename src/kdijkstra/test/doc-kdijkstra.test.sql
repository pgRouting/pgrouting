BEGIN;
------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------
--              PGR_kdijkstraPath
------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------
\echo -- q1
SELECT * FROM pgr_kdijkstraCost(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost FROM edge_table',
    10, array[4,12], false, false);
\echo -- q2
SELECT * FROM pgr_kdijkstraPath(
      'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost FROM edge_table',
      10, array[4,12], false, false);
\echo -- q3
SELECT id1 AS path, st_AStext(st_linemerge(st_union(b.the_geom))) AS the_geom
  FROM pgr_kdijkstraPath(
                  'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost FROM edge_table',
                  10, array[4,12], false, false
            ) a,
            edge_table b
WHERE a.id3=b.id
GROUP by id1
ORDER by id1;
\echo -- q4
ROLLBACK;
