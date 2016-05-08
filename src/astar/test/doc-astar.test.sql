BEGIN;
------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------
--              PGR_AStar
------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------
\echo --q1
SELECT * FROM pgr_astar(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, x1, y1, x2, y2 FROM edge_table',
    4, 1, false, false);
\echo --q2
SELECT * FROM pgr_astar(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost, x1, y1, x2, y2 FROM edge_table',
    4, 1, true, true);
\echo --q3
ROLLBACK;
