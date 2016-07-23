------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------
--              PGR_dijkstra  V.2.0
------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------
SELECT client_min_messages TO NOTICE;
\echo --q1
SELECT * FROM pgr_dijkstra(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table',
    2,3, true, false);
\echo --q2

SELECT * FROM pgr_dijkstra(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table',
    2,3, true, true);
\echo --q3

SELECT * FROM pgr_dijkstra(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost FROM edge_table',
     2, 3, false, false);
\echo --q4

SELECT * FROM pgr_dijkstra(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table',
     2, 3, false, true);
\echo --q5
