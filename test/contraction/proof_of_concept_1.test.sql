
\echo Adding an additional column to store if its part of the contracted graph
ALTER TABLE edge_table ADD is_contracted BOOLEAN DEFAULT false;

\echo Adding a new edges joining vertices 3 and 5 with cost=2 to the edge table
INSERT INTO edge_table(source, target, cost, reverse_cost, is_contracted)
VALUES (3, 5, 2, 2, true);

\echo Adding a new edges joining vertices 3 and 9 with cost=2 to the edge table
INSERT INTO edge_table(source, target, cost, reverse_cost, is_contracted)
VALUES (3, 9, 2, 2, true);

\echo Adding a new edges joining vertices 5 and 11 with cost=2 to the edge table
INSERT INTO edge_table(source, target, cost, reverse_cost, is_contracted)
VALUES (5, 11, 2, 2, true);

\echo Adding a new edges joining vertices 9 and 11 with cost=2 to the edge table
INSERT INTO edge_table(source, target, cost, reverse_cost, is_contracted)
VALUES (9, 11, 2, 2, true);

--\echo --q1
--SELECT id, source, target, cost, reverse_cost, is_contracted FROM edge_table;
--select id, source, target, cost, reverse_cost  from edge_table where source in (3,5,6,9,11,15,17) and target in (3,5,6,9,11,15,17);


\echo Case 1: Both source and target belong to the contracted graph.
\echo We are going to route from 3 to 11
\echo  Since 3 and 11 both are in the contracted graph we need not add any vertices.
SELECT * FROM
pgr_dijkstra('SELECT id, source, target, cost, reverse_cost FROM edge_table where source IN (3, 5, 6, 9, 11, 15, 17) AND target IN (3, 5, 6, 9, 11, 15, 17) AND is_contracted=false',
3, 11, false);


\echo Case 2: source belongs to a contracted graph, while target belongs to a vertex subgraph.
\echo We are going to route from 3 to 7
\echo  Since 7 is in the contracted subgraph of vertex 5 we add {7, 8} to the vertex set, so the vertex set becomes (3, 5, 6, 7, 8, 9, 11, 15, 17)
SELECT * FROM
pgr_dijkstra('SELECT id, source, target, cost, reverse_cost FROM edge_table where source IN (3, 5, 6, 9, 11, 15, 17,   7, 8)	 AND target IN (3, 5, 6, 9, 11, 15, 17,   7, 8) AND is_contracted=false',
3, 7, false);

\echo Case 3: source belongs to a contracted graph, while target belongs to a edge subgraph.
\echo We are going to route from 3 to 13
\echo  Since 13 is in the contracted subgraph of edge (5, 11) we add {10, 13} to the vertex set, so the vertex set becomes (3, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17)
SELECT * FROM
pgr_dijkstra('SELECT id, source, target, cost, reverse_cost FROM edge_table where source IN (3, 5, 6, 9, 11, 15, 17,    10, 13) AND target IN (3, 5, 6, 9, 11, 15, 17,    10, 13) AND is_contracted=false',
3, 13, false);

\echo Case 4: source belongs to a vertex subgraph, while target belongs to a edge subgraph.
\echo We are going to route from 7 to 13
\echo Since 13 is in the contracted subgraph of edge (5, 11) we add {10, 13} to the vertex set, and since 7 is in the contracted subgraph of vertex 5 we add {7, 8} to the vertex sets (3, 5, 6, 7, 8, 9, 11, 15, 17)
SELECT * FROM
pgr_dijkstra('SELECT id, source, target, cost, reverse_cost FROM edge_table where source IN (3, 5, 6, 9, 11, 15, 17,    7, 8,   10, 13) AND target IN (3, 5, 6, 9, 11, 15, 17,    7, 8,   10, 13) AND is_contracted=false',
7, 13, false);

\echo Case 5: The path contains a shortcut.
\echo \echo We are going to route from 3 to 9
\echo Since 3 and 9 both are in the contracted graph we need not add any vertices.
SELECT * FROM
pgr_dijkstra('SELECT id, source, target, cost, reverse_cost FROM edge_table where source IN (3, 5, 6, 7, 8, 9, 11, 15, 17) AND target IN (3, 5, 6, 7, 8, 9, 11, 15, 17) AND is_contracted=false',
3, 9, false);

SELECT id, source, target, cost, reverse_cost, is_contracted FROM edge_table ORDER BY id;

\echo This implies that it is a shortcut and should be expanded
SELECT * FROM
pgr_dijkstra('SELECT id, source, target, cost, reverse_cost FROM edge_table where source IN (3, 5, 6, 7, 8, 9, 11, 15, 17) AND target IN (3, 5, 6, 7, 8, 9, 11, 15, 17) AND is_contracted=false',
3, 9, false);

