BEGIN;
------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------
--              PGR_bdDijkstra
------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------

\echo --q1
SELECT * FROM pgr_bdDijkstra(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost FROM edge_table',
    4, 10, false, false);
\echo --q2
SELECT * FROM pgr_bdDijkstra(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table',
    4, 10, true, true);
\echo --q3
ROLLBACK;
