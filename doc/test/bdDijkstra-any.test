------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------
--              PGR_bdDijkstra
------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------

SELECT seq, id1 AS node, id2 AS edge, round(cost) as cost
    FROM pgr_bdDijkstra(
        'SELECT id, source, target, cost FROM edge_table order by id',
        7, 12, false, false);
SELECT seq, id1 AS node, id2 AS edge, cost
    FROM pgr_bdDijkstra(
        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
        7, 12, true, true);
