\i setup.sql

SELECT plan(30);

SET client_min_messages TO WARNING;
-- TESTING ONE CYCLE OF DEAD END CONTRACTION FOR AN UNDIRECTED GRAPH 

PREPARE qempty AS
SELECT seq, id, type, source, target, cost, unnest(contracted_vertices) AS contracted_vertices, contracted_vertices_size 
FROM ( VALUES (-1, -1, 'v' , -1, -1, -1, ARRAY[]::bigint[], 1) ) AS t(seq, id, type, source, target, cost, contracted_vertices, contracted_vertices_size)
WHERE 1 != 1;

-- SINGLE EDGE
-- no forbidden vertices
PREPARE v2e1q10 AS
SELECT seq, id, type, source, target, cost, unnest(contracted_vertices) AS contracted_vertices, contracted_vertices_size FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1',
    ARRAY[]::integer[], ARRAY[0]::integer[], 1, false);

PREPARE v2e1q11 AS
SELECT seq, id, type, source, target, cost, unnest(contracted_vertices) AS contracted_vertices, contracted_vertices_size  FROM ( VALUES (1, 2, 'v', -1, -1, -1, ARRAY[1]::bigint[], 1) ) AS t(seq, id, type, source, target, cost, contracted_vertices, contracted_vertices_size);

SELECT set_eq('v2e1q10', 'v2e1q11', '1: Undirected graph with single edge and no forbidden vertices');


-- 1 is forbidden vertex
PREPARE v2e1q20 AS
SELECT seq, id, type, source, target, cost, unnest(contracted_vertices) AS contracted_vertices, contracted_vertices_size  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1',
    ARRAY[1]::integer[], ARRAY[0]::integer[], 1, false);

PREPARE v2e1q21 AS
SELECT seq, id, type, source, target, cost, unnest(contracted_vertices) AS contracted_vertices, contracted_vertices_size  FROM ( VALUES (1, 1, 'v', -1, -1, -1, ARRAY[2]::bigint[], 1) ) AS t(seq, id, type, source, target, cost, contracted_vertices, contracted_vertices_size);
SELECT set_eq('v2e1q20', 'v2e1q21', '1: Undirected graph with single edge and 1 as forbidden vertex');

-- TWO EDGES
-- no forbidden vertices
PREPARE v3e2q10 AS
SELECT seq, id, type, source, target, cost, unnest(contracted_vertices) AS contracted_vertices, contracted_vertices_size  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 2',
    ARRAY[]::integer[], ARRAY[0]::integer[], 1, false);

PREPARE v3e2q11 AS
SELECT seq, id, type, source, target, cost, unnest(contracted_vertices) AS contracted_vertices, contracted_vertices_size  FROM ( VALUES (1, 3, 'v', -1, -1, -1,  ARRAY[1, 2]::bigint[], 2) ) AS t(seq, id, type, source, target, cost, contracted_vertices, contracted_vertices_size);

SELECT set_eq('v3e2q10', 'v3e2q11', '1: Undirected graph with single edge and no forbidden vertices');

-- 1 is forbidden vertex
PREPARE v3e2q20 AS
SELECT seq, id, type, source, target, cost, unnest(contracted_vertices) AS contracted_vertices, contracted_vertices_size  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 2',
    ARRAY[1]::integer[], ARRAY[0]::integer[], 1, false);
PREPARE v3e2q21 AS
SELECT seq, id, type, source, target, cost, unnest(contracted_vertices) AS contracted_vertices, contracted_vertices_size  FROM ( VALUES (1, 1, 'v', -1, -1, -1, ARRAY[2, 3]::bigint[], 2) ) AS t(seq, id, type, source, target, cost, contracted_vertices, contracted_vertices_size);

SELECT set_eq('v3e2q20', 'v3e2q21', '1: Undirected graph with two edges and 1 as forbidden vertex');

-- 2 is forbidden vertex
PREPARE v3e2q30 AS
SELECT seq, id, type, source, target, cost, unnest(contracted_vertices) AS contracted_vertices, contracted_vertices_size  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 2',
    ARRAY[2]::integer[], ARRAY[0]::integer[], 1, false);
PREPARE v3e2q31 AS
SELECT seq, id, type, source, target, cost, unnest(contracted_vertices) AS contracted_vertices, contracted_vertices_size  FROM ( VALUES (1, 2, 'v', -1, -1, -1,  ARRAY[1, 3]::bigint[], 2) ) AS t(seq, id, type, source, target, cost, contracted_vertices, contracted_vertices_size);

SELECT set_eq('v3e2q30', 'v3e2q31', '1: Undirected graph with two edges and 2 as forbidden vertex');

-- 3 is forbidden vertex
PREPARE v3e2q40 AS
SELECT seq, id, type, source, target, cost, unnest(contracted_vertices) AS contracted_vertices, contracted_vertices_size  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 2',
    ARRAY[3]::integer[], ARRAY[0]::integer[], 1, false);
PREPARE v3e2q41 AS
SELECT seq, id, type, source, target, cost, unnest(contracted_vertices) AS contracted_vertices, contracted_vertices_size  FROM ( VALUES (1, 3, 'v', -1, -1, -1,  ARRAY[1, 2]::bigint[], 2) ) AS t(seq, id, type, source, target, cost, contracted_vertices, contracted_vertices_size);

SELECT set_eq('v3e2q40', 'v3e2q41', '1: Undirected graph with two edges and 3 as forbidden vertex');

-- 1,2 are forbidden
PREPARE v3e2q50 AS
SELECT seq, id, type, source, target, cost, unnest(contracted_vertices) AS contracted_vertices, contracted_vertices_size  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 2',
    ARRAY[1, 2]::integer[], ARRAY[0]::integer[], 1, false);
PREPARE v3e2q51 AS
SELECT seq, id, type, source, target, cost, unnest(contracted_vertices) AS contracted_vertices, contracted_vertices_size  FROM ( VALUES (1, 2, 'v', -1, -1, -1,  ARRAY[3]::bigint[], 1) ) AS t(seq, id, type, source, target, cost, contracted_vertices, contracted_vertices_size);

SELECT set_eq('v3e2q50', 'v3e2q51', '1: Undirected graph with two edges and 1, 2 are forbidden vertices');

-- 3,2 are forbidden
PREPARE v3e2q60 AS
SELECT seq, id, type, source, target, cost, unnest(contracted_vertices) AS contracted_vertices, contracted_vertices_size  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 2',
    ARRAY[2, 3]::integer[], ARRAY[0]::integer[], 1, false);
PREPARE v3e2q61 AS
SELECT seq, id, type, source, target, cost, unnest(contracted_vertices) AS contracted_vertices, contracted_vertices_size  FROM ( VALUES (1, 2, 'v', -1, -1, -1,  ARRAY[1]::bigint[], 1) ) AS t(seq, id, type, source, target, cost, contracted_vertices, contracted_vertices_size);

SELECT set_eq('v3e2q60', 'v3e2q61', '1: Undirected graph with two edges and 3, 2 are forbidden vertices');

-- 1,3 are forbidden
PREPARE v3e2q70 AS
SELECT seq, id, type, source, target, cost, unnest(contracted_vertices) AS contracted_vertices, contracted_vertices_size  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 2',
    ARRAY[1, 3]::integer[], ARRAY[0]::integer[], 1, false);

SELECT set_eq('v3e2q70', 'qempty', '1: Undirected graph with two edges and 3, 1 are forbidden vertices');

-- no forbidden
PREPARE v3e2q80 AS
SELECT seq, id, type, source, target, cost, unnest(contracted_vertices) AS contracted_vertices, contracted_vertices_size  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 or id = 5',
    ARRAY[]::integer[], ARRAY[0]::integer[], 1, false);

PREPARE v3e2q81 AS
SELECT seq, id, type, source, target, cost, unnest(contracted_vertices) AS contracted_vertices, contracted_vertices_size  FROM ( VALUES (1, 7, 'v', -1, -1, -1,  ARRAY[5, 6]::bigint[], 2)) AS t(seq, id, type, source, target, cost, contracted_vertices, contracted_vertices_size);

SELECT set_eq('v3e2q80', 'v3e2q81', '4: Undirected graph with two edges and no forbidden vertices');

-- 5 is forbidden
PREPARE v3e2q90 AS
SELECT seq, id, type, source, target, cost, unnest(contracted_vertices) AS contracted_vertices, contracted_vertices_size  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 or id = 5',
    ARRAY[5]::integer[], ARRAY[0]::integer[], 1, false);

PREPARE v3e2q91 AS
SELECT seq, id, type, source, target, cost, unnest(contracted_vertices) AS contracted_vertices, contracted_vertices_size  FROM ( VALUES (1, 5, 'v', -1, -1, -1,  ARRAY[6, 7]::bigint[], 2)) AS t(seq, id, type, source, target, cost, contracted_vertices, contracted_vertices_size);

SELECT set_eq('v3e2q90', 'v3e2q91', '4: Undirected graph with two edges and 5 is forbidden vertex');

-- THREE EDGES
-- no forbidden vertices
PREPARE v4e3q10 AS
SELECT seq, id, type, source, target, cost, unnest(contracted_vertices) AS contracted_vertices, contracted_vertices_size  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 2 or id = 3',
    ARRAY[]::integer[], ARRAY[0]::integer[], 1, false);

PREPARE v4e3q11 AS
SELECT seq, id, type, source, target, cost, unnest(contracted_vertices) AS contracted_vertices, contracted_vertices_size  FROM ( VALUES (1, 4, 'v', -1, -1, -1, ARRAY[1, 2, 3]::bigint[], 3) ) AS t(seq, id, type, source, target, cost, contracted_vertices, contracted_vertices_size);

SELECT set_eq('v4e3q10', 'v4e3q11', '5: Undirected graph with three edges and no forbidden vertices');

-- 1 is forbidden
PREPARE v4e3q20 AS
SELECT seq, id, type, source, target, cost, unnest(contracted_vertices) AS contracted_vertices, contracted_vertices_size  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 2 or id = 3',
    ARRAY[1]::integer[], ARRAY[0]::integer[], 1, false);

PREPARE v4e3q21 AS
SELECT seq, id, type, source, target, cost, unnest(contracted_vertices) AS contracted_vertices, contracted_vertices_size  FROM ( VALUES (1, 1, 'v', -1, -1, -1,  ARRAY[2, 3, 4]::bigint[], 3) ) AS t(seq, id, type, source, target, cost, contracted_vertices, contracted_vertices_size);

SELECT set_eq('v4e3q20', 'v4e3q21', '5: Undirected graph with three edges and 1 as forbidden vertex');

-- 2 is forbidden
PREPARE v4e3q30 AS
SELECT seq, id, type, source, target, cost, unnest(contracted_vertices) AS contracted_vertices, contracted_vertices_size  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 2 or id = 3',
    ARRAY[2]::integer[], ARRAY[0]::integer[], 1, false);

PREPARE v4e3q31 AS
SELECT seq, id, type, source, target, cost, unnest(contracted_vertices) AS contracted_vertices, contracted_vertices_size  FROM ( VALUES (1, 2, 'v', -1, -1, -1,  ARRAY[1, 3, 4]::bigint[], 3) ) AS t(seq, id, type, source, target, cost, contracted_vertices, contracted_vertices_size);

SELECT set_eq('v4e3q30', 'v4e3q31', '5: Undirected graph with three edges and 2 as forbidden vertex');

-- 3 is forbidden
PREPARE v4e3q40 AS
SELECT seq, id, type, source, target, cost, unnest(contracted_vertices) AS contracted_vertices, contracted_vertices_size  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 2 or id = 3',
    ARRAY[3]::integer[], ARRAY[0]::integer[], 1, false);

PREPARE v4e3q41 AS
SELECT seq, id, type, source, target, cost, unnest(contracted_vertices) AS contracted_vertices, contracted_vertices_size  FROM ( VALUES (1, 3, 'v', -1, -1, -1,  ARRAY[1, 2, 4]::bigint[], 3) ) AS t(seq, id, type, source, target, cost, contracted_vertices, contracted_vertices_size);

SELECT set_eq('v4e3q40', 'v4e3q41', '5: Undirected graph with three edges and 3 as forbidden vertex');

-- no forbidden vertices
PREPARE v4e3q50 AS
SELECT seq, id, type, source, target, cost, unnest(contracted_vertices) AS contracted_vertices, contracted_vertices_size  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 or id = 5 or id = 6',
    ARRAY[]::integer[], ARRAY[0]::integer[], 1, false);

PREPARE v4e3q51 AS
SELECT seq, id, type, source, target, cost, unnest(contracted_vertices) AS contracted_vertices, contracted_vertices_size  FROM ( VALUES (1, 8, 'v', -1, -1, -1,  ARRAY[5, 6, 7]::bigint[], 3) ) AS t(seq, id, type, source, target, cost, contracted_vertices, contracted_vertices_size);

SELECT set_eq('v4e3q50', 'v4e3q51', '7: Undirected graph with three edges and no forbidden vertices');

-- 5 is forbidden
PREPARE v4e3q60 AS
SELECT seq, id, type, source, target, cost, unnest(contracted_vertices) AS contracted_vertices, contracted_vertices_size  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 or id = 5 or id = 6',
    ARRAY[5]::integer[], ARRAY[0]::integer[], 1, false);

PREPARE v4e3q61 AS
SELECT seq, id, type, source, target, cost, unnest(contracted_vertices) AS contracted_vertices, contracted_vertices_size  FROM ( VALUES (1, 5, 'v', -1, -1, -1,  ARRAY[6, 7, 8]::bigint[], 3) ) AS t(seq, id, type, source, target, cost, contracted_vertices, contracted_vertices_size);

SELECT set_eq('v4e3q60', 'v4e3q61', '7: Undirected graph with three edges and 5 is forbidden vertex');

-- 6 is forbidden
PREPARE v4e3q70 AS
SELECT seq, id, type, source, target, cost, unnest(contracted_vertices) AS contracted_vertices, contracted_vertices_size  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 or id = 5 or id = 6',
    ARRAY[6]::integer[], ARRAY[0]::integer[], 1, false);

PREPARE v4e3q71 AS
SELECT seq, id, type, source, target, cost, unnest(contracted_vertices) AS contracted_vertices, contracted_vertices_size  FROM ( VALUES (1, 6, 'v', -1, -1, -1,  ARRAY[5, 7, 8]::bigint[], 3) ) AS t(seq, id, type, source, target, cost, contracted_vertices, contracted_vertices_size);

SELECT set_eq('v4e3q70', 'v4e3q71', '7: Undirected graph with three edges and 6 is forbidden vertex');

-- 7 is forbidden
PREPARE v4e3q80 AS
SELECT seq, id, type, source, target, cost, unnest(contracted_vertices) AS contracted_vertices, contracted_vertices_size  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 or id = 5 or id = 6',
    ARRAY[7]::integer[], ARRAY[0]::integer[], 1, false);

PREPARE v4e3q81 AS
SELECT seq, id, type, source, target, cost, unnest(contracted_vertices) AS contracted_vertices, contracted_vertices_size  FROM ( VALUES (1, 7, 'v', -1, -1, -1,  ARRAY[5, 6, 8]::bigint[], 3) ) AS t(seq, id, type, source, target, cost, contracted_vertices, contracted_vertices_size);

SELECT set_eq('v4e3q80', 'v4e3q81', '7: Undirected graph with three edges and 7 is forbidden vertex');

-- 8 is forbidden
PREPARE v4e3q90 AS
SELECT seq, id, type, source, target, cost, unnest(contracted_vertices) AS contracted_vertices, contracted_vertices_size  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 or id = 5 or id = 6',
    ARRAY[8]::integer[], ARRAY[0]::integer[], 1, false);

PREPARE v4e3q91 AS
SELECT seq, id, type, source, target, cost, unnest(contracted_vertices) AS contracted_vertices, contracted_vertices_size  FROM ( VALUES (1, 8, 'v', -1, -1, -1,  ARRAY[5, 6, 7]::bigint[], 3) ) AS t(seq, id, type, source, target, cost, contracted_vertices, contracted_vertices_size);

SELECT set_eq('v4e3q90', 'v4e3q91', '7: Undirected graph with three edges and 8 is forbidden vertex');

-- FOUR EDGES
-- no forbidden vertices
PREPARE v4e4q10 AS
SELECT seq, id, type, source, target, cost, unnest(contracted_vertices) AS contracted_vertices, contracted_vertices_size  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 or id = 5 or id = 6 or id = 8',
    ARRAY[]::integer[], ARRAY[0]::integer[], 1, false);

PREPARE v4e4q11 AS
SELECT seq, id, type, source, target, cost, unnest(contracted_vertices) AS contracted_vertices, contracted_vertices_size  FROM ( VALUES (1, 5, 'v', -1, -1, -1, ARRAY[8]::bigint[], 1) ) AS t(seq, id, type, source, target, cost, contracted_vertices, contracted_vertices_size);

SELECT set_eq('v4e4q10', 'v4e4q11', '9: Undirected graph with four edges and no forbidden vertices');

-- no forbidden vertices
PREPARE v6e4q10 AS
SELECT seq, id, type, source, target, cost, unnest(contracted_vertices) AS contracted_vertices, contracted_vertices_size  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 2 or id = 3 or id = 4',
    ARRAY[]::integer[], ARRAY[0]::integer[], 1, false);

PREPARE v6e4q11 AS
SELECT seq, id, type, source, target, cost, unnest(contracted_vertices) AS contracted_vertices, contracted_vertices_size  FROM ( VALUES (1, 4, 'v', -1, -1, -1,  ARRAY[1, 2, 3]::bigint[], 3), (2, 6, 'v', -1, -1, -1,  ARRAY[5]::bigint[], 1) ) AS t(seq, id, type, source, target, cost, contracted_vertices, contracted_vertices_size);

SELECT set_eq('v6e4q10', 'v6e4q11', '11: Directed graph with four edges and no forbidden vertices');


-- 5 is forbidden
PREPARE v6e4q20 AS
SELECT seq, id, type, source, target, cost, unnest(contracted_vertices) AS contracted_vertices, contracted_vertices_size  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 2 or id = 3 or id = 4',
    ARRAY[5]::integer[], ARRAY[0]::integer[], 1, false);

PREPARE v6e4q21 AS
SELECT seq, id, type, source, target, cost, unnest(contracted_vertices) AS contracted_vertices, contracted_vertices_size  FROM ( VALUES (1, 4, 'v', -1, -1, -1, ARRAY[1, 2, 3]::bigint[], 3), (2, 5, 'v', -1, -1, -1, ARRAY[6]::bigint[], 1) ) AS t(seq, id, type, source, target, cost, contracted_vertices, contracted_vertices_size);

SELECT set_eq('v6e4q20', 'v6e4q21', '8: Directed graph with four edges and 5 is forbidden vertex');

-- 1 is forbidden
PREPARE v6e4q30 AS
SELECT seq, id, type, source, target, cost, unnest(contracted_vertices) AS contracted_vertices, contracted_vertices_size  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 2 or id = 3 or id = 4',
    ARRAY[1]::integer[], ARRAY[0]::integer[], 1, false);

PREPARE v6e4q31 AS
SELECT seq, id, type, source, target, cost, unnest(contracted_vertices) AS contracted_vertices, contracted_vertices_size  FROM ( VALUES (1, 1, 'v', -1, -1, -1, ARRAY[2, 3, 4]::bigint[], 3), (2, 6, 'v', -1, -1, -1, ARRAY[5]::bigint[], 1) ) AS t(seq, id, type, source, target, cost, contracted_vertices, contracted_vertices_size);

SELECT set_eq('v6e4q30', 'v6e4q31', '8: Directed graph with four edges and 1 is forbidden vertex');

-- 2 is forbidden
PREPARE v6e4q40 AS
SELECT seq, id, type, source, target, cost, unnest(contracted_vertices) AS contracted_vertices, contracted_vertices_size  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 2 or id = 3 or id = 4',
    ARRAY[2]::integer[], ARRAY[0]::integer[], 1, false);

PREPARE v6e4q41 AS
SELECT seq, id, type, source, target, cost, unnest(contracted_vertices) AS contracted_vertices, contracted_vertices_size  FROM ( VALUES (1, 2, 'v', -1, -1, -1, ARRAY[1, 3, 4]::bigint[], 3), (2, 6, 'v', -1, -1, -1, ARRAY[5]::bigint[], 1) ) AS t(seq, id, type, source, target, cost, contracted_vertices, contracted_vertices_size);

SELECT set_eq('v6e4q40', 'v6e4q41', '8: Directed graph with four edges and 2 is forbidden vertex');

-- 3 is forbidden
PREPARE v6e4q50 AS
SELECT seq, id, type, source, target, cost, unnest(contracted_vertices) AS contracted_vertices, contracted_vertices_size  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 2 or id = 3 or id = 4',
    ARRAY[3]::integer[], ARRAY[0]::integer[], 1, false);

PREPARE v6e4q51 AS
SELECT seq, id, type, source, target, cost, unnest(contracted_vertices) AS contracted_vertices, contracted_vertices_size  FROM ( VALUES (1, 3, 'v', -1, -1, -1, ARRAY[1, 2, 4]::bigint[], 3), (2, 6, 'v', -1, -1, -1, ARRAY[5]::bigint[], 1) ) AS t(seq, id, type, source, target, cost, contracted_vertices, contracted_vertices_size);

SELECT set_eq('v6e4q50', 'v6e4q51', '8: Directed graph with four edges and 3 is forbidden vertex');

-- 1, 2 are forbidden
PREPARE v6e4q60 AS
SELECT seq, id, type, source, target, cost, unnest(contracted_vertices) AS contracted_vertices, contracted_vertices_size  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 2 or id = 3 or id = 4',
    ARRAY[1, 2]::integer[], ARRAY[0]::integer[], 1, false);

PREPARE v6e4q61 AS
SELECT seq, id, type, source, target, cost, unnest(contracted_vertices) AS contracted_vertices, contracted_vertices_size  FROM ( VALUES (1, 2, 'v', -1, -1, -1, ARRAY[3, 4]::bigint[], 2), (2, 6, 'v', -1, -1, -1, ARRAY[5]::bigint[], 1) ) AS t(seq, id, type, source, target, cost, contracted_vertices, contracted_vertices_size);

SELECT set_eq('v6e4q60', 'v6e4q61', '8: Directed graph with four edges and 1, 2 are forbidden vertices');

-- 2, 3 are forbidden
PREPARE v6e4q70 AS
SELECT seq, id, type, source, target, cost, unnest(contracted_vertices) AS contracted_vertices, contracted_vertices_size  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 2 or id = 3 or id = 4',
    ARRAY[2, 3]::integer[], ARRAY[0]::integer[], 1, false);

PREPARE v6e4q71 AS
SELECT seq, id, type, source, target, cost, unnest(contracted_vertices) AS contracted_vertices, contracted_vertices_size  FROM ( VALUES (1, 2, 'v', -1, -1, -1, ARRAY[1]::bigint[], 1), (2, 3, 'v', -1, -1, -1, ARRAY[4]::bigint[], 1), (3, 6, 'v', -1, -1, -1, ARRAY[5]::bigint[], 1) ) AS t(seq, id, type, source, target, cost, contracted_vertices, contracted_vertices_size);

SELECT set_eq('v6e4q70', 'v6e4q71', '8: Directed graph with four edges and 2, 3 are forbidden vertices');


-- 1, 3 are forbidden
PREPARE v6e4q80 AS
SELECT seq, id, type, source, target, cost, unnest(contracted_vertices) AS contracted_vertices, contracted_vertices_size  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 2 or id = 3 or id = 4',
    ARRAY[1, 3]::integer[], ARRAY[0]::integer[], 1, false);

PREPARE v6e4q81 AS
SELECT seq, id, type, source, target, cost, unnest(contracted_vertices) AS contracted_vertices, contracted_vertices_size  FROM ( VALUES (1, 3, 'v', -1, -1, -1, ARRAY[4]::bigint[], 1), (2, 6, 'v', -1, -1, -1, ARRAY[5]::bigint[], 1) ) AS t(seq, id, type, source, target, cost, contracted_vertices, contracted_vertices_size);

SELECT set_eq('v6e4q80', 'v6e4q81', '8: Directed graph with four edges and 1, 3 are forbidden vertices');

-- 1, 5 are forbidden
PREPARE v6e4q90 AS
SELECT seq, id, type, source, target, cost, unnest(contracted_vertices) AS contracted_vertices, contracted_vertices_size  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 2 or id = 3 or id = 4',
    ARRAY[1, 5]::integer[], ARRAY[0]::integer[], 1, false);

PREPARE v6e4q91 AS
SELECT seq, id, type, source, target, cost, unnest(contracted_vertices) AS contracted_vertices, contracted_vertices_size  FROM ( VALUES (1, 1, 'v', -1, -1, -1, ARRAY[2, 3, 4]::bigint[], 3), (2, 5, 'v', -1, -1, -1, ARRAY[6]::bigint[], 1) ) AS t(seq, id, type, source, target, cost, contracted_vertices, contracted_vertices_size);

SELECT set_eq('v6e4q90', 'v6e4q91', '8: Directed graph with four edges and 1, 5 are forbidden vertices');


SELECT finish();
ROLLBACK;