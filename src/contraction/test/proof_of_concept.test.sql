ALTER TABLE edge_table ADD is_contracted BOOLEAN DEFAULT false;

INSERT INTO edge_table(source, target, cost, reverse_cost, is_contracted) 
VALUES (3, 5, 2, 2, true);

INSERT INTO edge_table(source, target, cost, reverse_cost, is_contracted) 
VALUES (3, 9, 2, 2, true);

INSERT INTO edge_table(source, target, cost, reverse_cost, is_contracted) 
VALUES (5, 11, 2, 2, true);

INSERT INTO edge_table(source, target, cost, reverse_cost, is_contracted) 
VALUES (9, 11, 2, 2, true);

--\echo --q1
--SELECT id, source, target, cost, reverse_cost, is_contracted FROM edge_table;



\echo --q2 
SELECT * FROM 
pgr_dijkstra('SELECT id, source, target, cost, reverse_cost FROM edge_table where source IN (3, 5, 6, 9, 11, 15, 17) AND target IN (3, 5, 6, 9, 11, 15, 17)',
3, 11, false);