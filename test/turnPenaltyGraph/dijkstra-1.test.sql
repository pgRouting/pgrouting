
-- this is a hand made test
UPDATE edge_table SET id = id * 100, source = 1000 * source, target = 1000 * target;
UPDATE edge_table_vertices_pgr SET id = id * 1000;

SELECT  *, 0 AS edge  INTO result1 FROM pgr_turnPenaltyGraph(
    $$SELECT id, source, target, cost, reverse_cost
    FROM edge_table
    WHERE id = 100$$
);
SELECT * FROM result1;

UPDATE result1 SET source = 1000 WHERE source = 1;
UPDATE result1 SET target = 1000 WHERE target = 1;


UPDATE result1 SET source = 2000 WHERE source = 3;
UPDATE result1 SET target = 2000 WHERE target = 3;

SELECT seq, edge, source, target, cost
INTO result2
FROM result1
ORDER BY seq;

-- original result
SELECT * from pgr_dijkstra($$SELECT id, * FROM edge_table$$,
    1000, 2000);

-- showing the 0 cost edges
SELECT * from pgr_dijkstra($$SELECT seq AS id, * FROM result2$$,
    1000, 2000);

-- not showing the 0 cost edges
SELECT * from pgr_dijkstra($$SELECT seq AS id, * FROM result2$$,
    1000, 2000) WHERE cost != 0 OR edge = -1 ORDER BY seq;

-- want the original result
SELECT * from pgr_dijkstra($$SELECT seq AS id, * FROM result2$$,
    1000, 2000) WHERE cost != 0 OR edge = -1 ORDER BY seq;

