\i setup.sql

SELECT plan(26);

SET client_min_messages TO WARNING;
-- TESTING ONE CYCLE OF DEAD END CONTRACTION FOR AN UNDIRECTED GRAPH 

PREPARE qempty AS
SELECT * FROM ( VALUES (-1, -1, 'v', -1, -1, '{}') ) AS t(seq, id, type, source, target, contracted_vertices) WHERE 1 != 1 ;

-- TWO EDGES
-- no forbidden vertices
PREPARE v3e2q10 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 2',
    ARRAY[]::integer[], ARRAY[1]::integer[], 1, false);
PREPARE v3e2q11 AS
SELECT * FROM ( VALUES (1, -1, 'e', 1, 3, '{2, }') ) AS t(seq, id, type, source, target, contracted_vertices);
SELECT set_eq('v3e2q10', 'v3e2q11', '1: Undirected graph with two edges and no forbidden vertices');

-- 2 is forbidden
PREPARE v3e2q20 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 2',
    ARRAY[2]::integer[], ARRAY[1]::integer[], 1, false);

SELECT set_eq('v3e2q20', 'qempty', '1: Undirected graph with two edges and 2 is forbidden vertex');

-- no forbidden vertices
PREPARE v3e2q30 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 or id = 5',
    ARRAY[]::integer[], ARRAY[1]::integer[], 1, false);
PREPARE v3e2q31 AS
SELECT * FROM ( VALUES (1, -1, 'e', 6, 7, '{5, }') ) AS t(seq, id, type, source, target, contracted_vertices);

SELECT set_eq('v3e2q30', 'v3e2q31', '1: Undirected graph with two edges and no forbidden vertex');


-- 5 is forbidden
PREPARE v3e2q40 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 2',
    ARRAY[5]::integer[], ARRAY[1]::integer[], 1, false);

SELECT set_eq('v3e2q40', 'qempty', '1: Undirected graph with two edges and 5 is forbidden vertex');


-- THREE EDGES
-- no forbidden vertices
PREPARE v4e3q10 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 2 or id = 3',
    ARRAY[]::integer[], ARRAY[1]::integer[], 1, false);

PREPARE v4e3q11 AS
SELECT * FROM ( VALUES (1, -1, 'e', 1, 3, '{2, }'), (2, -2, 'e', 1, 4, '{2, 3, }') ) AS t(seq, id, type, source, target, contracted_vertices);

SELECT set_eq('v4e3q10', 'v4e3q11', '5: Undirected graph with three edges and no forbidden vertices');

-- 2 is forbidden
PREPARE v4e3q20 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 2 or id = 3',
    ARRAY[2]::integer[], ARRAY[1]::integer[], 1, false);

PREPARE v4e3q21 AS
SELECT * FROM ( VALUES (1, -1, 'e', 2, 4, '{3, }') ) AS t(seq, id, type, source, target, contracted_vertices);

SELECT set_eq('v4e3q20', 'v4e3q21', '5: Undirected graph with three edges and 2 is forbidden vertices');


-- 3 is forbidden
PREPARE v4e3q30 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 2 or id = 3',
    ARRAY[3]::integer[], ARRAY[1]::integer[], 1, false);

PREPARE v4e3q31 AS
SELECT * FROM ( VALUES (1, -1, 'e', 1, 3, '{2, }') ) AS t(seq, id, type, source, target, contracted_vertices);

SELECT set_eq('v4e3q30', 'v4e3q31', '5: Undirected graph with three edges and 3 is forbidden vertices');

-- 2, 3 are forbidden
PREPARE v4e3q40 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 2 or id = 3',
    ARRAY[2, 3]::integer[], ARRAY[1]::integer[], 1, false);

SELECT set_eq('v4e3q40', 'qempty', '5: Undirected graph with three edges and 3 is forbidden vertices');

-- FOUR EDGES
-- no forbidden vertices
PREPARE v4e4q10 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 9 or id = 10 or id = 11',
    ARRAY[]::integer[], ARRAY[1]::integer[], 1, false);

PREPARE v4e4q11 AS
SELECT * FROM ( VALUES (1, -1, 'e', 2, 9, '{1, }'), (2, -2, 'e', 9, 10, '{1, 2, }') ) AS t(seq, id, type, source, target, contracted_vertices);

SELECT set_eq('v4e4q10', 'v4e4q11', '5: Undirected graph with four edges and no forbidden vertices');


-- 1 is forbidden
PREPARE v4e4q20 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 9 or id = 10 or id = 11',
    ARRAY[1]::integer[], ARRAY[1]::integer[], 1, false);

PREPARE v4e4q21 AS
SELECT * FROM ( VALUES (1, -1, 'e', 1, 10, '{2, }'), (2, -2, 'e', 1, 10, '{9, }') ) AS t(seq, id, type, source, target, contracted_vertices);

SELECT set_eq('v4e4q20', 'v4e4q21', '5: Undirected graph with four edges and 1 is forbidden vertices');

-- 2 is forbidden
PREPARE v4e4q30 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 9 or id = 10 or id = 11',
    ARRAY[2]::integer[], ARRAY[1]::integer[], 1, false);

PREPARE v4e4q31 AS
SELECT * FROM ( VALUES (1, -1, 'e', 2, 9, '{1, }'), (2, -2, 'e', 2, 10, '{1, 9, }') ) AS t(seq, id, type, source, target, contracted_vertices);

SELECT set_eq('v4e4q30', 'v4e4q31', '5: Undirected graph with four edges and 2 is forbidden vertices');

-- 9 is forbidden
PREPARE v4e4q40 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 9 or id = 10 or id = 11',
    ARRAY[9]::integer[], ARRAY[1]::integer[], 1, false);

PREPARE v4e4q41 AS
SELECT * FROM ( VALUES (1, -1, 'e', 2, 9, '{1, }'), (2, -2, 'e', 9, 10, '{1, 2, }') ) AS t(seq, id, type, source, target, contracted_vertices);

SELECT set_eq('v4e4q40', 'v4e4q41', '5: Undirected graph with four edges and 9 is forbidden vertices');

-- 1, 2 are forbidden
PREPARE v4e4q50 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 9 or id = 10 or id = 11',
    ARRAY[1, 2]::integer[], ARRAY[1]::integer[], 1, false);

PREPARE v4e4q51 AS
SELECT * FROM ( VALUES (1, -1, 'e', 1, 10, '{9, }'), (2, -2, 'e', 1, 2, '{9, 10, }') ) AS t(seq, id, type, source, target, contracted_vertices);

SELECT set_eq('v4e4q50', 'v4e4q51', '5: Undirected graph with four edges and 1, 2 are vertices');

-- 1, 9 are forbidden
PREPARE v4e4q60 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 9 or id = 10 or id = 11',
    ARRAY[1, 9]::integer[], ARRAY[1]::integer[], 1, false);

PREPARE v4e4q61 AS
SELECT * FROM ( VALUES (1, -1, 'e', 1, 10, '{2, }'), (2, -2, 'e', 1, 9, '{2, 10, }') ) AS t(seq, id, type, source, target, contracted_vertices);

SELECT set_eq('v4e4q60', 'v4e4q61', '5: Undirected graph with four edges and 1, 9 are vertices');

-- 2, 9 are forbidden
PREPARE v4e4q70 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 9 or id = 10 or id = 11',
    ARRAY[2, 9]::integer[], ARRAY[1]::integer[], 1, false);

PREPARE v4e4q71 AS
SELECT * FROM ( VALUES (1, -1, 'e', 2, 9, '{1, }'), (2, -2, 'e', 2, 9, '{10, }') ) AS t(seq, id, type, source, target, contracted_vertices);

SELECT set_eq('v4e4q70', 'v4e4q71', '5: Undirected graph with four edges and 2, 9 are vertices');


-- 1, 2, 9 are forbidden
PREPARE v4e4q80 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 9 or id = 10 or id = 11',
    ARRAY[1, 2, 9]::integer[], ARRAY[1]::integer[], 1, false);

PREPARE v4e4q81 AS
SELECT * FROM ( VALUES (1, -1, 'e', 2, 9, '{10, }') ) AS t(seq, id, type, source, target, contracted_vertices);

SELECT set_eq('v4e4q80', 'v4e4q81', '5: Undirected graph with four edges and 1, 2, 9 are vertices');

-- no forbidden vertices
PREPARE v4e4q90 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 2 or id = 9 or id = 12 or id = 13',
    ARRAY[]::integer[], ARRAY[1]::integer[], 1, false);

PREPARE v4e4q91 AS
SELECT * FROM ( VALUES (1, -1, 'e', 3, 10, '{2, }'), (2, -2, 'e', 10, 11, '{2, 3, }') ) AS t(seq, id, type, source, target, contracted_vertices);

SELECT set_eq('v4e4q90', 'v4e4q91', '5: Directed graph with four edges and no forbidden vertices');

-- 2 is forbidden
PREPARE v4e4q100 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 2 or id = 9 or id = 12 or id = 13',
    ARRAY[2]::integer[], ARRAY[1]::integer[], 1, false);

PREPARE v4e4q101 AS
SELECT * FROM ( VALUES (1, -1, 'e', 2, 11, '{3, }'), (2, -2, 'e', 2, 11, '{10, }') ) AS t(seq, id, type, source, target, contracted_vertices);

SELECT set_eq('v4e4q100', 'v4e4q101', '5: Directed graph with four edges and 2 is forbidden vertices');


-- 3 is forbidden
PREPARE v4e4q110 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 2 or id = 9 or id = 12 or id = 13',
    ARRAY[3]::integer[], ARRAY[1]::integer[], 1, false);

PREPARE v4e4q111 AS
SELECT * FROM ( VALUES (1, -1, 'e', 3, 10, '{2, }'), (2, -2, 'e', 3, 11, '{2, 10, }') ) AS t(seq, id, type, source, target, contracted_vertices);

SELECT set_eq('v4e4q110', 'v4e4q111', '5: Directed graph with four edges and 3 is forbidden vertices');


-- 10 is forbidden
PREPARE v4e4q120 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 2 or id = 9 or id = 12 or id = 13',
    ARRAY[10]::integer[], ARRAY[1]::integer[], 1, false);

PREPARE v4e4q121 AS
SELECT * FROM ( VALUES (1, -1, 'e', 3, 10, '{2, }'), (2, -2, 'e', 10, 11, '{2, 3, }') ) AS t(seq, id, type, source, target, contracted_vertices);

SELECT set_eq('v4e4q120', 'v4e4q121', '5: Directed graph with four edges and 10 is forbidden vertices');

-- 11 is forbidden
PREPARE v4e4q130 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 2 or id = 9 or id = 12 or id = 13',
    ARRAY[11]::integer[], ARRAY[1]::integer[], 1, false);

PREPARE v4e4q131 AS
SELECT * FROM ( VALUES (1, -1, 'e', 3, 10, '{2, }'), (2, -2, 'e', 10, 11, '{2, 3, }') ) AS t(seq, id, type, source, target, contracted_vertices);

SELECT set_eq('v4e4q130', 'v4e4q131', '5: Directed graph with four edges and 11 is forbidden vertices');

-- 2, 3 are forbidden
PREPARE v4e4q140 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 2 or id = 9 or id = 12 or id = 13',
    ARRAY[2, 3]::integer[], ARRAY[1]::integer[], 1, false);

PREPARE v4e4q141 AS
SELECT * FROM ( VALUES (1, -1, 'e', 2, 11, '{10, }'), (2, -2, 'e', 2, 3, '{10, 11, }') ) AS t(seq, id, type, source, target, contracted_vertices);

SELECT set_eq('v4e4q140', 'v4e4q141', '5: Directed graph with four edges and 2, 3 are forbidden vertices');

-- 2, 10 are forbidden
PREPARE v4e4q150 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 2 or id = 9 or id = 12 or id = 13',
    ARRAY[2, 10]::integer[], ARRAY[1]::integer[], 1, false);

PREPARE v4e4q151 AS
SELECT * FROM ( VALUES (1, -1, 'e', 2, 11, '{3, }'), (2, -2, 'e', 2, 10, '{3, 11, }') ) AS t(seq, id, type, source, target, contracted_vertices);

SELECT set_eq('v4e4q150', 'v4e4q151', '5: Directed graph with four edges and 2, 10 are forbidden vertices');

-- 2, 11 are forbidden
PREPARE v4e4q160 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 2 or id = 9 or id = 12 or id = 13',
    ARRAY[2, 11]::integer[], ARRAY[1]::integer[], 1, false);

PREPARE v4e4q161 AS
SELECT * FROM ( VALUES (1, -1, 'e', 2, 11, '{3, }'), (2, -2, 'e', 2, 11, '{10, }') ) AS t(seq, id, type, source, target, contracted_vertices);

SELECT set_eq('v4e4q160', 'v4e4q161', '5: Directed graph with four edges and 2, 11 are forbidden vertices');


-- 3, 10 are forbidden
PREPARE v4e4q170 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 2 or id = 9 or id = 12 or id = 13',
    ARRAY[3, 10]::integer[], ARRAY[1]::integer[], 1, false);

PREPARE v4e4q171 AS
SELECT * FROM ( VALUES (1, -1, 'e', 3, 10, '{2, }'), (2, -2, 'e', 3, 10, '{11, }') ) AS t(seq, id, type, source, target, contracted_vertices);

SELECT set_eq('v4e4q170', 'v4e4q171', '5: Directed graph with four edges and 3, 10 are forbidden vertices');

-- 3, 11 are forbidden
PREPARE v4e4q180 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 2 or id = 9 or id = 12 or id = 13',
    ARRAY[3, 11]::integer[], ARRAY[1]::integer[], 1, false);

PREPARE v4e4q181 AS
SELECT * FROM ( VALUES (1, -1, 'e', 3, 10, '{2, }'), (2, -2, 'e', 3, 11, '{2, 10, }') ) AS t(seq, id, type, source, target, contracted_vertices);

SELECT set_eq('v4e4q180', 'v4e4q181', '5: Directed graph with four edges and 3, 11 are forbidden vertices');







SELECT finish();
ROLLBACK;