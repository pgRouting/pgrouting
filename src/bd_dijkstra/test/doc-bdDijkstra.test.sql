BEGIN;
SET client_min_messages TO NOTICE;
------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------
--              PGR_bdDijkstra
------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------

\echo --q1
SELECT * FROM pgr_bdDijkstra(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost FROM edge_table',
    2, 3, false, false);
\echo --q2
SELECT * FROM pgr_bdDijkstra(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table',
    2, 3, true, true);
\echo --q3
ROLLBACK;
