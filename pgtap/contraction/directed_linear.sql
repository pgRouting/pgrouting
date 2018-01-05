\i setup.sql

SELECT plan(15);

SET client_min_messages TO WARNING;
-- TESTING ONE CYCLE OF DEAD END CONTRACTION FOR AN UNDIRECTED GRAPH

PREPARE qempty AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM ( VALUES (-1, 'e', -1, ARRAY[]::bigint[], -1, -1, -1) ) AS t(seq, type, id, contracted_vertices, source, target, cost) WHERE 1 != 1 ;

-- TWO EDGES
-- no forbidden vertices
PREPARE v3e2q10 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 2',
    ARRAY[2]::integer[], 1, ARRAY[]::INTEGER[], true);

PREPARE v3e2q11 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM ( VALUES (1, 'e', -1, ARRAY[2]::bigint[], 3, 1, 2) ) AS t(seq, type, id, contracted_vertices, source, target, cost);
SELECT set_eq('v3e2q10', 'v3e2q11', '1: Directed graph with two edges and no forbidden vertices');

-- 2 is forbidden
PREPARE v3e2q20 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 2',
    ARRAY[2]::INTEGER[], 1, ARRAY[2]::integer[], true);

SELECT set_eq('v3e2q20', 'qempty', '1: Directed graph with two edges and 2 is forbidden vertex');


-- no forbidden vertices
PREPARE v3e2q30 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 11 or id = 12',
    ARRAY[2]::integer[], 1, ARRAY[]::INTEGER[], true);

SELECT set_eq('v3e2q30', 'qempty', '1: Directed graph with two edges and no forbidden vertex');

-- THREE EDGES
-- no forbidden vertices
PREPARE v4e3q10 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 3 or id = 5 or id = 11',
    ARRAY[2]::integer[], 1, ARRAY[]::INTEGER[], true);

PREPARE v4e3q11 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM ( VALUES (1, 'e', -1, ARRAY[3]::bigint[], 4, 6, 2), (2, 'e', -2, ARRAY[3, 6]::bigint[], 4, 11, 3) ) AS t(seq, type, id, contracted_vertices, source, target, cost);

SELECT set_eq('v4e3q10', 'v4e3q11', '5: Directed graph with three edges and no forbidden vertices');

-- 3 is forbidden
PREPARE v4e3q20 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 3 or id = 5 or id = 11',
    ARRAY[2]::INTEGER[], 1, ARRAY[3]::integer[], true);

PREPARE v4e3q21 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM ( VALUES (1, 'e', -1, ARRAY[6]::bigint[], 3, 11, 2) ) AS t(seq, type, id, contracted_vertices, source, target, cost);

SELECT set_eq('v4e3q20', 'v4e3q21', '5: Directed graph with three edges and 3 is forbidden vertices');

-- 6 is forbidden
PREPARE v4e3q30 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 3 or id = 5 or id = 11',
    ARRAY[2]::INTEGER[], 1, ARRAY[6]::integer[], true);

PREPARE v4e3q31 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM ( VALUES (1, 'e', -1, ARRAY[3]::bigint[], 4, 6, 2) ) AS t(seq, type, id, contracted_vertices, source, target, cost);

SELECT set_eq('v4e3q30', 'v4e3q31', '5: Directed graph with three edges and 6 is forbidden vertices');

-- 3, 6 are forbidden
PREPARE v4e3q40 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 3 or id = 5 or id = 11',
    ARRAY[2]::INTEGER[], 1, ARRAY[3, 6]::integer[], true);

SELECT set_eq('v4e3q40', 'qempty', '5: Directed graph with three edges and 3, 6 are forbidden vertices');




-- FOUR EDGES
-- no forbidden vertices
PREPARE v4e4q10 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 9 or id = 11 or id = 13 or id = 15',
    ARRAY[2]::integer[], 1, ARRAY[]::INTEGER[], true);

PREPARE v4e4q11 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM ( VALUES (1, 'e', -1, ARRAY[6]::bigint[], 9, 11, 2), (2, 'e', -2, ARRAY[6, 9]::bigint[], 12, 11, 3) ) AS t(seq, type, id, contracted_vertices, source, target, cost);

SELECT set_eq('v4e4q10', 'v4e4q11', '5: Directed graph with four edges and no forbidden vertices');

-- 6 is forbidden
PREPARE v4e4q20 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 9 or id = 11 or id = 13 or id = 15',
    ARRAY[2]::INTEGER[], 1, ARRAY[6]::integer[], true);

PREPARE v4e4q21 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM ( VALUES (1, 'e', -1, ARRAY[9]::bigint[], 6, 12, 2), (2, 'e', -2, ARRAY[9]::bigint[], 12, 6, 2), (3, 'e', -3, ARRAY[11]::bigint[], 6, 12, 2) ) AS t(seq, type, id, contracted_vertices, source, target, cost);

SELECT set_eq('v4e4q20', 'v4e4q21', '5: Directed graph with four edges and 6 is forbidden vertices');

-- 9 is forbidden
PREPARE v4e4q30 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 9 or id = 11 or id = 13 or id = 15',
    ARRAY[2]::INTEGER[], 1, ARRAY[9]::integer[], true);

PREPARE v4e4q31 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM ( VALUES (1, 'e', -1, ARRAY[6]::bigint[], 9, 11, 2), (2, 'e', -2, ARRAY[6, 11]::bigint[], 9, 12, 3) ) AS t(seq, type, id, contracted_vertices, source, target, cost);

SELECT set_eq('v4e4q30', 'v4e4q31', '5: Directed graph with four edges and 9 is forbidden vertices');

-- 11 is forbidden
PREPARE v4e4q40 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 9 or id = 11 or id = 13 or id = 15',
    ARRAY[2]::INTEGER[], 1, ARRAY[11]::integer[], true);

PREPARE v4e4q41 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM ( VALUES (1, 'e', -1, ARRAY[6]::bigint[], 9, 11, 2), (2, 'e', -2, ARRAY[6, 9]::bigint[], 12, 11, 3) ) AS t(seq, type, id, contracted_vertices, source, target, cost);

SELECT set_eq('v4e4q40', 'v4e4q41', '5: Directed graph with four edges and 11 is forbidden vertices');

-- 6, 9 are forbidden
PREPARE v4e4q50 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 9 or id = 11 or id = 13 or id = 15',
    ARRAY[2]::INTEGER[], 1, ARRAY[6, 9]::integer[], true);

PREPARE v4e4q51 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM ( VALUES (1, 'e', -1, ARRAY[11]::bigint[], 6, 12, 2), (2, 'e', -2, ARRAY[11, 12]::bigint[], 6, 9, 3) ) AS t(seq, type, id, contracted_vertices, source, target, cost);

SELECT set_eq('v4e4q50', 'v4e4q51', '5: Directed graph with four edges and 6, 9 are forbidden vertices');


-- 9, 11 are forbidden
PREPARE v4e4q60 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 9 or id = 11 or id = 13 or id = 15',
    ARRAY[2]::INTEGER[], 1, ARRAY[9, 11]::integer[], true);

PREPARE v4e4q61 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM ( VALUES (1, 'e', -1, ARRAY[6]::bigint[], 9, 11, 2), (2, 'e', -2, ARRAY[12]::bigint[], 11, 9, 2) ) AS t(seq, type, id, contracted_vertices, source, target, cost);

SELECT set_eq('v4e4q60', 'v4e4q61', '5: Directed graph with four edges and 9, 11 are forbidden vertices');

-- 6, 11 are forbidden
PREPARE v4e4q70 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 9 or id = 11 or id = 13 or id = 15',
    ARRAY[2]::INTEGER[], 1, ARRAY[6, 11]::integer[], true);

PREPARE v4e4q71 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM ( VALUES (1, 'e', -1, ARRAY[9]::bigint[], 6, 12, 2), (2, 'e', -2, ARRAY[9]::bigint[], 12, 6, 2), (3, 'e', -3, ARRAY[9, 12]::bigint[], 11, 6, 3) ) AS t(seq, type, id, contracted_vertices, source, target, cost);

SELECT set_eq('v4e4q70', 'v4e4q71', '5: Directed graph with four edges and 6, 11 are forbidden vertices');

-- 6, 9, 11 are forbidden
PREPARE v4e4q80 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 9 or id = 11 or id = 13 or id = 15',
    ARRAY[2]::INTEGER[], 1, ARRAY[6, 9, 11]::integer[], true);

PREPARE v4e4q81 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM ( VALUES (1, 'e', -1, ARRAY[12]::bigint[], 11, 9, 2) ) AS t(seq, type, id, contracted_vertices, source, target, cost);

SELECT set_eq('v4e4q80', 'v4e4q81', '5: Directed graph with four edges and 1, 2, 9 are vertices');




SELECT finish();
ROLLBACK;