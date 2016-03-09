BEGIN;
------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------
--              PGR_pgr_bdAStar
------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------
\echo --q1
SELECT * FROM pgr_bdAStar(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, x1, y1, x2, y2
     FROM edge_table',
    4, 10, false, false);
\echo --q2
SELECT * FROM pgr_bdAStar(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, x1, y1, x2, y2, reverse_cost
     FROM edge_table ',
    4, 10, true, true);
\echo --q3
ROLLBACK;
