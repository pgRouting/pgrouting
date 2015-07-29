------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------
--              PGR_drivingDistance
------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------
SELECT id1 AS node, cost
        FROM pgr_drivingDistance(
                'SELECT id, source, target, cost FROM edge_table',
                6, 1.5, false, false
        ) order by id1;

SELECT id1 AS node, cost
        FROM pgr_drivingDistance(
                'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                7, 1.5, true, true
        ) order by id1;
