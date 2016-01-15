BEGIN;
------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------
--              PGR_dijkstra  V.2.0
------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------

SELECT seq, id1 AS node, id2 AS edge, cost
        FROM pgr_dijkstra(
                'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                2,3, true, false
        );

SELECT seq, id1 AS node, id2 AS edge, cost
        FROM pgr_dijkstra(
                'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                2,3, true, true
        );

SELECT seq, id1 AS node, id2 AS edge, cost
         FROM pgr_dijkstra(
                 'SELECT id, source, target, cost FROM edge_table',
                 2, 3, false, false
         );

SELECT seq, id1 AS node, id2 AS edge, cost
         FROM pgr_dijkstra(
                 'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                 2, 3, false, true
         );
ROLLBACK;
