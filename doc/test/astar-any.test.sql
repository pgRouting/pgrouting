BEGIN;
------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------
--              PGR_AStar
------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------
SELECT seq, id1 AS node, id2 AS edge, cost
        FROM pgr_AStar(
                'SELECT id, source, target, cost, x1, y1, x2, y2 FROM edge_table order by id',
                4, 10, false, false);
SELECT seq, id1 AS node, id2 AS edge, round(cost) as cost
        FROM pgr_AStar(
                'SELECT id, source, target, cost, x1, y1, x2, y2, reverse_cost FROM edge_table ',
                4, 10, true, true);
ROLLBACK;
