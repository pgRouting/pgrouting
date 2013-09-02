------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------
--              PGR_drivingDistance
------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------
SELECT seq, id1 AS node, cost
        FROM pgr_drivingDistance(
                'SELECT id, source, target, cost FROM edge_table',
                6, 1.5, false, false
        );

SELECT seq, id1 AS node, cost
        FROM pgr_drivingDistance(
                'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                6, 1.5, true, true
        );

