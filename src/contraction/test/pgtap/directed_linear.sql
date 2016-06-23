\i setup.sql

SELECT plan(14);

SET client_min_messages TO WARNING;
-- TESTING ONE CYCLE OF DEAD END CONTRACTION FOR AN UNDIRECTED GRAPH 

PREPARE qempty AS
SELECT * FROM ( VALUES (-1, -1, 'v', -1, -1, '{}') ) AS t(seq, id, type, source, target, contracted_vertices) WHERE 1 != 1 ;

-- TWO EDGES
-- no forbidden vertices
PREPARE v3e2q10 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 2',
    ARRAY[]::integer[], ARRAY[1]::integer[], 1, true);

PREPARE v3e2q11 AS
SELECT * FROM ( VALUES (1, -1, 'e', 1, 3, '{2, }') ) AS t(seq, id, type, source, target, contracted_vertices);
SELECT set_eq('v3e2q10', 'v3e2q11', '1: Directed graph with two edges and no forbidden vertices');

-- 2 is forbidden
PREPARE v3e2q20 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 2',
    ARRAY[2]::integer[], ARRAY[1]::integer[], 1, true);

SELECT set_eq('v3e2q20', 'qempty', '1: Directed graph with two edges and 2 is forbidden vertex');


-- no forbidden vertices
PREPARE v3e2q30 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 or id = 5',
    ARRAY[]::integer[], ARRAY[1]::integer[], 1, true);

SELECT set_eq('v3e2q30', 'qempty', '1: Directed graph with two edges and no forbidden vertex');

-- THREE EDGES
-- no forbidden vertices
PREPARE v4e3q10 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 2 or id = 3',
    ARRAY[]::integer[], ARRAY[1]::integer[], 1, true);

PREPARE v4e3q11 AS
SELECT * FROM ( VALUES (1, -2, 'e', 1, 4, '{2, 3, }') ) AS t(seq, id, type, source, target, contracted_vertices);

SELECT set_eq('v4e3q10', 'v4e3q11', '5: Directed graph with three edges and no forbidden vertices');

-- 2 is forbidden
PREPARE v4e3q20 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 2 or id = 3',
    ARRAY[2]::integer[], ARRAY[1]::integer[], 1, true);

PREPARE v4e3q21 AS
SELECT * FROM ( VALUES (1, -1, 'e', 2, 4, '{3, }') ) AS t(seq, id, type, source, target, contracted_vertices);

SELECT set_eq('v4e3q20', 'v4e3q21', '5: Directed graph with three edges and 2 is forbidden vertices');

-- 3 is forbidden
PREPARE v4e3q30 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 2 or id = 3',
    ARRAY[3]::integer[], ARRAY[1]::integer[], 1, true);

PREPARE v4e3q31 AS
SELECT * FROM ( VALUES (1, -1, 'e', 1, 3, '{2, }') ) AS t(seq, id, type, source, target, contracted_vertices);

SELECT set_eq('v4e3q30', 'v4e3q31', '5: Directed graph with three edges and 1 is forbidden vertices');

-- FOUR EDGES
-- no forbidden vertices
PREPARE v4e4q10 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 9 or id = 10 or id = 11',
    ARRAY[]::integer[], ARRAY[1]::integer[], 1, true);

PREPARE v4e4q11 AS
SELECT * FROM ( VALUES (1, -2, 'e', 9, 10, '{1, 2, }') ) AS t(seq, id, type, source, target, contracted_vertices);

SELECT set_eq('v4e4q10', 'v4e4q11', '5: Directed graph with four edges and no forbidden vertices');

-- 1 is forbidden
PREPARE v4e4q20 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 9 or id = 10 or id = 11',
    ARRAY[1]::integer[], ARRAY[1]::integer[], 1, true);

PREPARE v4e4q21 AS
SELECT * FROM ( VALUES (1, -1, 'e', 1, 10, '{2, }'), (2, -2, 'e', 10, 1, '{9, }') ) AS t(seq, id, type, source, target, contracted_vertices);

SELECT set_eq('v4e4q20', 'v4e4q21', '5: Directed graph with four edges and no forbidden vertices');

-- 2 is forbidden
PREPARE v4e4q30 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 9 or id = 10 or id = 11',
    ARRAY[2]::integer[], ARRAY[1]::integer[], 1, true);

PREPARE v4e4q31 AS
SELECT * FROM ( VALUES (1, -2, 'e', 10, 2, '{1, 9, }') ) AS t(seq, id, type, source, target, contracted_vertices);

SELECT set_eq('v4e4q30', 'v4e4q31', '5: Directed graph with four edges and no forbidden vertices');

-- 9 is forbidden
PREPARE v4e4q40 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 9 or id = 10 or id = 11',
    ARRAY[9]::integer[], ARRAY[1]::integer[], 1, true);

PREPARE v4e4q41 AS
SELECT * FROM ( VALUES (1, -2, 'e', 9, 10, '{1, 2, }') ) AS t(seq, id, type, source, target, contracted_vertices);

SELECT set_eq('v4e4q40', 'v4e4q41', '5: Directed graph with four edges and no forbidden vertices');

-- 1, 2 are forbidden
PREPARE v4e4q50 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 9 or id = 10 or id = 11',
    ARRAY[1, 2]::integer[], ARRAY[1]::integer[], 1, true);

PREPARE v4e4q51 AS
SELECT * FROM ( VALUES (1, -2, 'e', 2, 1, '{9, 10, }') ) AS t(seq, id, type, source, target, contracted_vertices);

SELECT set_eq('v4e4q50', 'v4e4q51', '5: Directed graph with four edges and 1, 2 are vertices');


-- 1, 9 are forbidden
PREPARE v4e4q60 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 9 or id = 10 or id = 11',
    ARRAY[1, 9]::integer[], ARRAY[1]::integer[], 1, true);

PREPARE v4e4q61 AS
SELECT * FROM ( VALUES (1, -2, 'e', 1, 9, '{2, 10, }') ) AS t(seq, id, type, source, target, contracted_vertices);

SELECT set_eq('v4e4q60', 'v4e4q61', '5: Directed graph with four edges and 1, 9 are vertices');

-- 2, 9 are forbidden
PREPARE v4e4q70 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 9 or id = 10 or id = 11',
    ARRAY[2, 9]::integer[], ARRAY[1]::integer[], 1, true);

PREPARE v4e4q71 AS
SELECT * FROM ( VALUES (1, -1, 'e', 9, 2, '{1, }'), (2, -2, 'e', 2, 9, '{10, }') ) AS t(seq, id, type, source, target, contracted_vertices);

SELECT set_eq('v4e4q70', 'v4e4q71', '5: Directed graph with four edges and 2, 9 are vertices');

-- 1, 2, 9 are forbidden
PREPARE v4e4q80 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 9 or id = 10 or id = 11',
    ARRAY[1, 2, 9]::integer[], ARRAY[1]::integer[], 1, true);

PREPARE v4e4q81 AS
SELECT * FROM ( VALUES (1, -1, 'e', 2, 9, '{10, }') ) AS t(seq, id, type, source, target, contracted_vertices);

SELECT set_eq('v4e4q80', 'v4e4q81', '5: Directed graph with four edges and 1, 2, 9 are vertices');


SELECT finish();
ROLLBACK;