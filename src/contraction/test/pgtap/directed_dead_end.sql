\i setup.sql

SELECT plan(40);

SET client_min_messages TO WARNING;
-- TESTING ONE CYCLE OF DEAD END CONTRACTION FOR A DIRECTED GRAPH 

PREPARE qempty AS
SELECT * FROM ( VALUES (-1, -1, 'v', -1, -1, '{}') ) AS t(seq, id, type, source, target, contracted_vertices) WHERE 1 != 1 ;

-- SINGLE EDGE
-- no forbidden vertices
PREPARE v2e1q10 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1',
    ARRAY[]::integer[], ARRAY[0]::integer[], 1, true);

PREPARE v2e1q11 AS
SELECT * FROM ( VALUES (1, 1, 'v', -1, -1, '{2, }') ) AS t(seq, id, type, source, target, contracted_vertices);

SELECT set_eq('v2e1q10', 'v2e1q11', '1: Directed graph with single edge and no forbidden vertices');

-- 2 is forbidden vertex
PREPARE v2e1q20 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1',
    ARRAY[2]::integer[], ARRAY[0]::integer[], 1, true);

SELECT set_eq('v2e1q20', 'qempty', '1: Directed graph with single edge and 2 as forbidden vertex');

-- TWO EDGES
-- no forbidden vertices
PREPARE v3e2q10 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 2',
    ARRAY[]::integer[], ARRAY[0]::integer[], 1, true);

PREPARE v3e2q11 AS
SELECT * FROM ( VALUES (1, 1, 'v', -1, -1, '{2, 3, }') ) AS t(seq, id, type, source, target, contracted_vertices);
SELECT set_eq('v3e2q10', 'v3e2q11', '1: Directed graph with single edge and no forbidden vertices');

-- 3 is forbidden
PREPARE v3e2q20 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 2',
    ARRAY[3]::integer[], ARRAY[0]::integer[], 1, true);

SELECT set_eq('v3e2q20', 'qempty', '2: Directed graph with two edges and vertex 3 as forbidden vertex');

-- 2 is forbidden
PREPARE v3e2q30 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 2',
    ARRAY[2]::integer[], ARRAY[0]::integer[], 1, true);

PREPARE v3e2q31 AS
SELECT * FROM ( VALUES (1, 2, 'v', -1, -1, '{3, }') ) AS t(seq, id, type, source, target, contracted_vertices);

SELECT set_eq('v3e2q30', 'v3e2q31', '2: Directed graph with two edges and vertex 2 as forbidden vertex');

-- no forbidden vertices
PREPARE v3e2q40 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 or id = 5',
    ARRAY[]::integer[], ARRAY[0]::integer[], 1, true);

PREPARE v3e2q41 AS
SELECT * FROM ( VALUES (1, 6, 'v', -1, -1, '{5, }'), (2, 7, 'v', -1, -1, '{5, }') ) AS t(seq, id, type, source, target, contracted_vertices);
SELECT set_eq('v3e2q40', 'v3e2q41', '3: Directed graph with two edges and no forbidden vertices');

-- 5 is forbidden
PREPARE v3e2q50 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 or id = 5',
    ARRAY[5]::integer[], ARRAY[0]::integer[], 1, true);

SELECT set_eq('v3e2q50', 'qempty', '3: Directed graph with two edges and vertex 5 as forbidden vertex');

-- no forbidden
PREPARE v3e2q60 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 3',
    ARRAY[]::integer[], ARRAY[0]::integer[], 1, true);

PREPARE v3e2q61 AS
SELECT * FROM ( VALUES (1, 1, 'v', -1, -1, '{2, }'), (2, 3, 'v', -1, -1, '{4, }') ) AS t(seq, id, type, source, target, contracted_vertices);

SELECT set_eq('v3e2q60', 'v3e2q61', '4: Directed graph with two edges and no forbidden vertices');

-- 2 is forbidden
PREPARE v3e2q70 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 3',
    ARRAY[2]::integer[], ARRAY[0]::integer[], 1, true);

PREPARE v3e2q71 AS
SELECT * FROM ( VALUES (1, 3, 'v', -1, -1, '{4, }') ) AS t(seq, id, type, source, target, contracted_vertices);

SELECT set_eq('v3e2q70', 'v3e2q71', '4: Directed graph with two edges and 2 is forbidden vertex');

-- 4 is forbidden
PREPARE v3e2q80 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 3',
    ARRAY[4]::integer[], ARRAY[0]::integer[], 1, true);

PREPARE v3e2q81 AS
SELECT * FROM ( VALUES (1, 1, 'v', -1, -1, '{2, }') ) AS t(seq, id, type, source, target, contracted_vertices);

SELECT set_eq('v3e2q80', 'v3e2q81', '4: Directed graph with two edges and 2 is forbidden vertex');


-- THREE EDGES
-- no forbidden vertices
PREPARE v4e3q10 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 2 or id = 3',
    ARRAY[]::integer[], ARRAY[0]::integer[], 1, true);

PREPARE v4e3q11 AS
SELECT * FROM ( VALUES (1, 1, 'v', -1, -1, '{2, 3, 4, }') ) AS t(seq, id, type, source, target, contracted_vertices);

SELECT set_eq('v4e3q10', 'v4e3q11', '5: Directed graph with three edges and no forbidden vertices');

-- 4 is forbidden
PREPARE v4e3q20 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 2 or id = 3',
    ARRAY[4]::integer[], ARRAY[0]::integer[], 1, true);

PREPARE v4e3q21 AS
SELECT * FROM ( VALUES (-1, -1, 'v', -1, -1, '{}') ) AS t(seq, id, type, source, target, contracted_vertices) WHERE 1 != 1 ;

SELECT set_eq('v4e3q20', 'v4e3q21', '5: Directed graph with three edges and 4 is forbidden vertex');

-- 3 is forbidden
PREPARE v4e3q30 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 2 or id = 3',
    ARRAY[3]::integer[], ARRAY[0]::integer[], 1, true);

PREPARE v4e3q31 AS
SELECT * FROM ( VALUES (1, 3, 'v', -1, -1, '{4, }') ) AS t(seq, id, type, source, target, contracted_vertices);

SELECT set_eq('v4e3q30', 'v4e3q31', '5: Directed graph with three edges and 3 is forbidden vertex');

-- 2 is forbidden
PREPARE v4e3q40 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 2 or id = 3',
    ARRAY[2]::integer[], ARRAY[0]::integer[], 1, true);

PREPARE v4e3q41 AS
SELECT * FROM ( VALUES (1, 2, 'v', -1, -1, '{3, 4, }') ) AS t(seq, id, type, source, target, contracted_vertices);

SELECT set_eq('v4e3q40', 'v4e3q41', '5: Directed graph with three edges and 2 is forbidden vertex');

-- no forbidden vertices
PREPARE v4e3q50 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 or id = 5 or id = 6',
    ARRAY[]::integer[], ARRAY[0]::integer[], 1, true);

PREPARE v4e3q51 AS
SELECT * FROM ( VALUES (1, 6, 'v', -1, -1, '{5, }'), (2, 7, 'v', -1, -1, '{5, }'), (3, 8, 'v', -1, -1, '{5, }') ) AS t(seq, id, type, source, target, contracted_vertices);

SELECT set_eq('v4e3q50', 'v4e3q51', '6: Directed graph with three edges and no forbidden vertices');

-- 5 is forbidden
PREPARE v4e3q60 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 or id = 5 or id = 6',
    ARRAY[5]::integer[], ARRAY[0]::integer[], 1, true);

SELECT set_eq('v4e3q60', 'qempty', '6: Directed graph with three edges and 5 is forbidden vertex');

-- no forbidden vertices
PREPARE v5e3q10 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 2 or id = 4',
    ARRAY[]::integer[], ARRAY[0]::integer[], 1, true);

PREPARE v5e3q11 AS
SELECT * FROM ( VALUES (1, 1, 'v', -1, -1, '{2, 3, }'), (2, 6, 'v', -1, -1, '{5, }') ) AS t(seq, id, type, source, target, contracted_vertices);

SELECT set_eq('v5e3q10', 'v5e3q11', '7: Directed graph with three edges and no forbidden vertices');

-- 5 is forbidden
PREPARE v5e3q20 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 2 or id = 4',
    ARRAY[5]::integer[], ARRAY[0]::integer[], 1, true);

PREPARE v5e3q21 AS
SELECT * FROM ( VALUES (1, 1, 'v', -1, -1, '{2, 3, }') ) AS t(seq, id, type, source, target, contracted_vertices);

SELECT set_eq('v5e3q20', 'v5e3q21', '7: Directed graph with three edges and 5 is forbidden vertex');

-- 3 is forbidden
PREPARE v5e3q30 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 2 or id = 4',
    ARRAY[3]::integer[], ARRAY[0]::integer[], 1, true);

PREPARE v5e3q31 AS
SELECT * FROM ( VALUES (1, 6, 'v', -1, -1, '{5, }') ) AS t(seq, id, type, source, target, contracted_vertices);

SELECT set_eq('v5e3q30', 'v5e3q31', '7: Directed graph with three edges and 3 is forbidden vertex');

-- 2 is forbidden
PREPARE v5e3q40 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 2 or id = 4',
    ARRAY[2]::integer[], ARRAY[0]::integer[], 1, true);

PREPARE v5e3q41 AS
SELECT * FROM ( VALUES (1, 2, 'v', -1, -1, '{3, }'), (2, 6, 'v', -1, -1, '{5, }') ) AS t(seq, id, type, source, target, contracted_vertices);

SELECT set_eq('v5e3q40', 'v5e3q41', '7: Directed graph with three edges and 2 is forbidden vertex');

-- 3 and 5 are forbidden
PREPARE v5e3q50 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 2 or id = 4',
    ARRAY[3, 5]::integer[], ARRAY[0]::integer[], 1, true);

SELECT set_eq('v5e3q50', 'qempty', '7: Directed graph with three edges and 3 and 5 are forbidden vertices');

-- 2 and 5 are forbidden
PREPARE v5e3q60 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 2 or id = 4',
    ARRAY[2, 5]::integer[], ARRAY[0]::integer[], 1, true);
PREPARE v5e3q61 AS
SELECT * FROM ( VALUES (1, 2, 'v', -1, -1, '{3, }')) AS t(seq, id, type, source, target, contracted_vertices);
SELECT set_eq('v5e3q60', 'v5e3q61', '7: Directed graph with three edges and 2 and 5 are forbidden vertices');

-- no forbidden vertices
PREPARE v6e3q10 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 3 or id = 4',
    ARRAY[]::integer[], ARRAY[0]::integer[], 1, true);

PREPARE v6e3q11 AS
SELECT * FROM ( VALUES (1, 1, 'v', -1, -1, '{2, }'), (2, 3, 'v', -1, -1, '{4, }'), (3, 6, 'v', -1, -1, '{5, }') ) AS t(seq, id, type, source, target, contracted_vertices);

SELECT set_eq('v6e3q10', 'v6e3q11', '8: Directed graph with three edges and no forbidden vertices');

-- 5 is forbidden
PREPARE v6e3q20 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 3 or id = 4',
    ARRAY[5]::integer[], ARRAY[0]::integer[], 1, true);

PREPARE v6e3q21 AS
SELECT * FROM ( VALUES (1, 1, 'v', -1, -1, '{2, }'), (2, 3, 'v', -1, -1, '{4, }') ) AS t(seq, id, type, source, target, contracted_vertices);

SELECT set_eq('v6e3q20', 'v6e3q21', '8: Directed graph with three edges and 5 is forbidden vertex');

-- 4 is forbidden
PREPARE v6e3q30 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 3 or id = 4',
    ARRAY[4]::integer[], ARRAY[0]::integer[], 1, true);

PREPARE v6e3q31 AS
SELECT * FROM ( VALUES (1, 1, 'v', -1, -1, '{2, }'), (2, 6, 'v', -1, -1, '{5, }') ) AS t(seq, id, type, source, target, contracted_vertices);

SELECT set_eq('v6e3q30', 'v6e3q31', '8: Directed graph with three edges and 4 is forbidden vertex');

-- 2 is forbidden
PREPARE v6e3q40 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 3 or id = 4',
    ARRAY[2]::integer[], ARRAY[0]::integer[], 1, true);

PREPARE v6e3q41 AS
SELECT * FROM ( VALUES (1, 3, 'v', -1, -1, '{4, }'), (2, 6, 'v', -1, -1, '{5, }') ) AS t(seq, id, type, source, target, contracted_vertices);

SELECT set_eq('v6e3q40', 'v6e3q41', '8: Directed graph with three edges and 2 is forbidden vertex');

-- 5 and 4 are forbidden
PREPARE v6e3q50 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 3 or id = 4',
    ARRAY[4, 5]::integer[], ARRAY[0]::integer[], 1, true);

PREPARE v6e3q51 AS
SELECT * FROM ( VALUES (1, 1, 'v', -1, -1, '{2, }') ) AS t(seq, id, type, source, target, contracted_vertices);

SELECT set_eq('v6e3q50', 'v6e3q51', '8: Directed graph with three edges and 4 and 5 are forbidden vertices');

-- 5 and 2 are forbidden
PREPARE v6e3q60 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 3 or id = 4',
    ARRAY[2, 5]::integer[], ARRAY[0]::integer[], 1, true);

PREPARE v6e3q61 AS
SELECT * FROM ( VALUES (1, 3, 'v', -1, -1, '{4, }') ) AS t(seq, id, type, source, target, contracted_vertices);

SELECT set_eq('v6e3q60', 'v6e3q61', '8: Directed graph with three edges and 2 and 5 are forbidden vertices');

-- 4 and 2 are forbidden
PREPARE v6e3q70 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 3 or id = 4',
    ARRAY[4, 2]::integer[], ARRAY[0]::integer[], 1, true);

PREPARE v6e3q71 AS
SELECT * FROM ( VALUES (1, 6, 'v', -1, -1, '{5, }') ) AS t(seq, id, type, source, target, contracted_vertices);

SELECT set_eq('v6e3q70', 'v6e3q71', '8: Directed graph with three edges and 2 and 5 are forbidden vertices');


-- FOUR EDGES
-- no forbidden vertices
PREPARE v4e4q10 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 or id = 5 or id = 6 or id = 8',
    ARRAY[]::integer[], ARRAY[0]::integer[], 1, true);

PREPARE v4e4q11 AS
SELECT * FROM ( VALUES (1, 7, 'v', -1, -1, '{5, 6, }'), (2, 8, 'v', -1, -1, '{5, }') ) AS t(seq, id, type, source, target, contracted_vertices);

SELECT set_eq('v4e4q10', 'v4e4q11', '9: Directed graph with four edges and no forbidden vertices');

-- 5 is forbidden
PREPARE v4e4q20 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 or id = 5 or id = 6 or id = 8',
    ARRAY[5]::integer[], ARRAY[0]::integer[], 1, true);

SELECT set_eq('v4e4q20', 'qempty', '9: Directed graph with four edges and 5 is forbidden vertex');

-- 6 is forbidden
PREPARE v4e4q30 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 or id = 5 or id = 6 or id = 8',
    ARRAY[6]::integer[], ARRAY[0]::integer[], 1, true);

PREPARE v4e4q31 AS
SELECT * FROM ( VALUES (1, 6, 'v', -1, -1, '{5, }'), (2, 7, 'v', -1, -1, '{5, }'), (3, 8, 'v', -1, -1, '{5, }') ) AS t(seq, id, type, source, target, contracted_vertices);
SELECT set_eq('v4e4q30', 'v4e4q31', '9: Directed graph with four edges and 5 is forbidden vertex');


-- no forbidden vertices
PREPARE v6e4q10 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 2 or id = 3 or id = 4',
    ARRAY[]::integer[], ARRAY[0]::integer[], 1, true);

PREPARE v6e4q11 AS
SELECT * FROM ( VALUES (1, 1, 'v', -1, -1, '{2, 3, 4, }'), (2, 6, 'v', -1, -1, '{5, }') ) AS t(seq, id, type, source, target, contracted_vertices);

SELECT set_eq('v6e4q10', 'v6e4q11', '11: Directed graph with four edges and no forbidden vertices');

-- 5 is forbidden
PREPARE v6e4q20 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 2 or id = 3 or id = 4',
    ARRAY[5]::integer[], ARRAY[0]::integer[], 1, true);

PREPARE v6e4q21 AS
SELECT * FROM ( VALUES (1, 1, 'v', -1, -1, '{2, 3, 4, }') ) AS t(seq, id, type, source, target, contracted_vertices);

SELECT set_eq('v6e4q20', 'v6e4q21', '8: Directed graph with four edges and 5 is forbidden vertex');

-- 4 is forbidden
PREPARE v6e4q30 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 2 or id = 3 or id = 4',
    ARRAY[4]::integer[], ARRAY[0]::integer[], 1, true);

PREPARE v6e4q31 AS
SELECT * FROM ( VALUES (1, 6, 'v', -1, -1, '{5, }') ) AS t(seq, id, type, source, target, contracted_vertices);

SELECT set_eq('v6e4q30', 'v6e4q31', '8: Directed graph with four edges and 4 is forbidden vertex');

-- 3 is forbidden
PREPARE v6e4q40 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 2 or id = 3 or id = 4',
    ARRAY[3]::integer[], ARRAY[0]::integer[], 1, true);

PREPARE v6e4q41 AS
SELECT * FROM ( VALUES (1, 3, 'v', -1, -1, '{4, }'), (2, 6, 'v', -1, -1, '{5, }') ) AS t(seq, id, type, source, target, contracted_vertices);

SELECT set_eq('v6e4q40', 'v6e4q41', '8: Directed graph with four edges and 3 is forbidden vertex');

-- 2 is forbidden
PREPARE v6e4q50 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 2 or id = 3 or id = 4',
    ARRAY[2]::integer[], ARRAY[0]::integer[], 1, true);

PREPARE v6e4q51 AS
SELECT * FROM ( VALUES (1, 2, 'v', -1, -1, '{3, 4, }'), (2, 6, 'v', -1, -1, '{5, }') ) AS t(seq, id, type, source, target, contracted_vertices);

SELECT set_eq('v6e4q50', 'v6e4q51', '8: Directed graph with four edges and 2 is forbidden vertex');


-- 5, 4 are forbidden
PREPARE v6e4q60 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 2 or id = 3 or id = 4',
    ARRAY[4, 5]::integer[], ARRAY[0]::integer[], 1, true);

SELECT set_eq('v6e4q60', 'qempty', '8: Directed graph with four edges and 4, 5 are forbidden vertex');

-- 5, 3 are forbidden
PREPARE v6e4q70 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 2 or id = 3 or id = 4',
    ARRAY[3, 5]::integer[], ARRAY[0]::integer[], 1, true);

PREPARE v6e4q71 AS
SELECT * FROM ( VALUES (1, 3, 'v', -1, -1, '{4, }') ) AS t(seq, id, type, source, target, contracted_vertices);

SELECT set_eq('v6e4q70', 'v6e4q71', '8: Directed graph with four edges and 3 is forbidden vertex');

-- 5, 2 are forbidden
PREPARE v6e4q80 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 2 or id = 3 or id = 4',
    ARRAY[2, 5]::integer[], ARRAY[0]::integer[], 1, true);

PREPARE v6e4q81 AS
SELECT * FROM ( VALUES (1, 2, 'v', -1, -1, '{3, 4, }')) AS t(seq, id, type, source, target, contracted_vertices);

SELECT set_eq('v6e4q80', 'v6e4q81', '8: Directed graph with four edges and 2 is forbidden vertex');

SELECT finish();
ROLLBACK;