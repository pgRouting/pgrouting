\echo -- q1
SELECT id, source, target, cost, reverse_cost FROM edge_table;

\echo -- q2
ALTER TABLE edge_table ADD is_contracted BOOLEAN DEFAULT false;

\echo -- q3
ALTER TABLE edge_table ADD contracted_vertices integer[];

\echo -- q4
SELECT id, source, target, cost, reverse_cost, is_contracted,
contracted_vertices FROM edge_table;

\echo -- q5
INSERT INTO edge_table(id, source, target, cost, reverse_cost, is_contracted, contracted_vertices) VALUES (-1, 3, 5, 2, 2, true, ARRAY[1, 2]);

\echo -- q6
INSERT INTO edge_table(id, source, target, cost, reverse_cost, is_contracted, contracted_vertices) VALUES (-2, 3, 9, 2, 2, true, ARRAY[4]);

\echo -- q7
INSERT INTO edge_table(id, source, target, cost, reverse_cost, is_contracted, contracted_vertices) VALUES (-3, 5, 11, 2, 2, true, ARRAY[10, 13]);

\echo -- q8
INSERT INTO edge_table(id, source, target, cost, reverse_cost, is_contracted, contracted_vertices) VALUES (-4, 9, 11, 2, 2, true, ARRAY[12]);

\echo -- q9
SELECT id, source, target, cost, reverse_cost, is_contracted, contracted_vertices FROM edge_table;

\echo -- q10
SELECT id, source, target, cost, reverse_cost, is_contracted, contracted_vertices FROM edge_table where source IN (3, 5, 6, 9, 11, 15, 17) AND target IN (3, 5, 6, 9, 11, 15, 17);

\echo -- q11
SELECT * FROM pgr_dijkstra(
      'SELECT id, source, target, cost, reverse_cost 
       FROM edge_table 
       WHERE source IN (3, 5, 6, 9, 11, 15, 17) 
       AND target IN (3, 5, 6, 9, 11, 15, 17)',
       3, 11, false);

\echo -- q12
SELECT  *  FROM  pgr_dijkstra(
   'SELECT id, source, target, cost, reverse_cost
    FROM edge_table
    WHERE source IN (3, 5, 6, 9, 11, 15, 17,   7, 8)
    AND target IN (3, 5, 6, 9, 11, 15, 17,   7, 8)',
    3, 7, false);

\echo -- q13
SELECT * FROM pgr_dijkstra(
  'SELECT id, source, target, cost, reverse_cost
   FROM edge_table
   WHERE source IN (3, 5, 6, 9, 11, 15, 17,    10, 13)
   AND target IN (3, 5, 6, 9, 11, 15, 17,    10, 13)',
   3, 13, false);

\echo -- q14
SELECT * FROM pgr_dijkstra(
  'SELECT id, source, target, cost, reverse_cost
   FROM edge_table
   WHERE source IN (3, 5, 6, 9, 11, 15, 17,    7, 8,   10, 13)
   AND target IN (3, 5, 6, 9, 11, 15, 17,    7, 8,   10, 13)',
   7, 13, false);

\echo -- q15
SELECT * FROM pgr_dijkstra(
    'SELECT id, source, target, cost, reverse_cost 
    FROM edge_table 
    WHERE source IN (3, 5, 6, 9, 11, 15, 17)
    AND target IN (3, 5, 6, 9, 11, 15, 17)',
    3, 9, false);

\echo -- q16
SELECT is_contracted FROM edge_table WHERE id = -2;

\echo -- q17
SELECT * FROM pgr_dijkstra(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table 
    WHERE source IN (3, 5, 6, 9, 11, 15, 17,  4) 
    AND target IN (3, 5, 6, 9, 11, 15, 17,  4) 
    AND is_contracted = false',
    3, 9, false);

\echo -- q18