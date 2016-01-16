------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------
--              PGR_drivingDistance
------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------
BEGIN;
\echo --q1
SELECT * FROM pgr_drivingDistance(
    'SELECT id, source, target, cost FROM edge_table',
    7, 1.5, false, false
) ;

SELECT * FROM pgr_drivingDistance(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    7, 1.5, true, true
) ;
\echo --q2
ROLLBACK;
