\i setup.sql

SELECT plan(39);

SET client_min_messages TO WARNING;
-- TESTING ONE CYCLE OF DEAD END CONTRACTION FOR A DIRECTED GRAPH 

PREPARE qempty AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  
FROM ( VALUES (-1, 'v', -1, ARRAY[]::bigint[], -1, -1, -1) ) AS t(seq, type, id, contracted_vertices, source, target, cost )
WHERE 1 != 1;

-- SINGLE EDGE
-- no forbidden vertices
PREPARE v2e1q10 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost 
FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1',
    ARRAY[]::integer[], ARRAY[1]::integer[], 1, true);

PREPARE v2e1q11 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  
FROM ( VALUES (1, 'v', 2, ARRAY[1]::bigint[], -1, -1, -1) ) AS t(seq, type, id, contracted_vertices, source, target, cost ) ;

SELECT set_eq('v2e1q10', 'v2e1q11', '1: Directed graph with single edge and no forbidden vertices');

-- 1 is forbidden vertex

PREPARE v2e1q20 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost 
FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1',
    ARRAY[1]::integer[], ARRAY[1]::INTEGER[], 1, true);

PREPARE v2e1q21 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  
FROM ( VALUES (1, 'v', 1, ARRAY[2]::bigint[], -1, -1, -1) ) AS t(seq, type, id, contracted_vertices, source, target, cost ) ;


SELECT set_eq('v2e1q20', 'v2e1q21', '1: Directed graph with single edge and 1 as forbidden vertex');


-- TWO EDGES
-- no forbidden vertices
PREPARE v3e2q10 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost 
FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 2 OR id = 3',
    ARRAY[]::integer[], ARRAY[1]::INTEGER[], 1, true);

PREPARE v3e2q11 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  
FROM ( VALUES (1, 'v', 4, ARRAY[2, 3]::bigint[], -1, -1, -1) ) AS t(seq, type, id, contracted_vertices, source, target, cost ) ;

SELECT set_eq('v3e2q10', 'v3e2q11', '1: Directed graph two edges and no forbidden vertices');

-- 3 is forbidden
PREPARE v3e2q20 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 2 OR id = 3',
    ARRAY[3]::integer[], ARRAY[1]::INTEGER[], 1, true);

PREPARE v3e2q21 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  
FROM ( VALUES (1, 'v', 3, ARRAY[2]::bigint[], -1, -1, -1) ) AS t(seq, type, id, contracted_vertices, source, target, cost ) ;

SELECT set_eq('v3e2q20', 'v3e2q21', '2: Directed graph with two edges and vertex 3 as forbidden vertex');

-- 2 is forbidden
PREPARE v3e2q30 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 2 OR id = 3',
    ARRAY[2]::integer[], ARRAY[1]::INTEGER[], 1, true);

SELECT set_eq('v3e2q30', 'qempty', '2: Directed graph with two edges and vertex 2 as forbidden vertex');

-- no forbidden vertices
PREPARE v3e2q40 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 11 or id = 12',
    ARRAY[]::integer[], ARRAY[1]::INTEGER[], 1, true);

PREPARE v3e2q41 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM ( VALUES (1, 'v', 6, ARRAY[11]::bigint[], -1, -1, -1), (2, 'v', 10, ARRAY[11]::integer[], -1, -1, -1) ) AS t(seq, type, id, contracted_vertices, source, target, cost );
SELECT set_eq('v3e2q40', 'v3e2q41', '3: Directed graph with two edges and no forbidden vertices');

-- 11 is forbidden
PREPARE v3e2q50 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 11 or id = 12',
    ARRAY[11]::integer[], ARRAY[1]::INTEGER[], 1, true);

SELECT set_eq('v3e2q50', 'qempty', '3: Directed graph with two edges and vertex 11 as forbidden vertex');

-- no forbidden
PREPARE v3e2q60 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 2 or id = 12',
    ARRAY[]::integer[], ARRAY[1]::INTEGER[], 1, true);

PREPARE v3e2q61 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM ( VALUES (1, 'v', 3, ARRAY[2]::bigint[], -1, -1, -1), (2, 'v', 10, ARRAY[11]::integer[], -1, -1, -1) ) AS t(seq, type, id, contracted_vertices, source, target, cost );

SELECT set_eq('v3e2q60', 'v3e2q61', '4: Directed graph with two edges and no forbidden vertices');

-- 2 is forbidden
PREPARE v3e2q70 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 2 or id = 12',
    ARRAY[2]::integer[], ARRAY[1]::INTEGER[], 1, true);

PREPARE v3e2q71 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM ( VALUES (1, 'v', 10, ARRAY[11]::bigint[], -1, -1, -1) ) AS t(seq, type, id, contracted_vertices, source, target, cost );

SELECT set_eq('v3e2q70', 'v3e2q71', '4: Directed graph with two edges and 2 is forbidden vertex');

-- 11 is forbidden
PREPARE v3e2q80 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 2 or id = 12',
    ARRAY[11]::integer[], ARRAY[1]::INTEGER[], 1, true);

PREPARE v3e2q81 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM ( VALUES (1, 'v', 3, ARRAY[2]::bigint[], -1, -1, -1) ) AS t(seq, type, id, contracted_vertices, source, target, cost );

SELECT set_eq('v3e2q80', 'v3e2q81', '4: Directed graph with two edges and 11 is forbidden vertex');


-- THREE EDGES
-- no forbidden vertices
PREPARE v4e3q10 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 5 or id = 11 or id = 13',
    ARRAY[]::integer[], ARRAY[1]::INTEGER[], 1, true);

PREPARE v4e3q11 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM ( VALUES (1, 'v', 3, ARRAY[6, 11, 12]::bigint[], -1, -1, -1) ) AS t(seq, type, id, contracted_vertices, source, target, cost );

SELECT set_eq('v4e3q10', 'v4e3q11', '5: Directed graph with three edges and no forbidden vertices');

-- 3 is forbidden
PREPARE v4e3q20 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 5 or id = 11 or id = 13',
    ARRAY[3]::integer[], ARRAY[1]::INTEGER[], 1, true);

PREPARE v4e3q21 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM ( VALUES (1, 'v', 3, ARRAY[6, 11, 12]::bigint[], -1, -1, -1) ) AS t(seq, type, id, contracted_vertices, source, target, cost );

SELECT set_eq('v4e3q20', 'v4e3q21', '5: Directed graph with three edges and 3 is forbidden vertex');

-- 6 is forbidden
PREPARE v4e3q30 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 5 or id = 11 or id = 13',
    ARRAY[6]::integer[], ARRAY[1]::INTEGER[], 1, true);

PREPARE v4e3q31 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM ( VALUES (1, 'v', 6, ARRAY[11, 12]::bigint[], -1, -1, -1) ) AS t(seq, type, id, contracted_vertices, source, target, cost );

SELECT set_eq('v4e3q30', 'v4e3q31', '5: Directed graph with three edges and 6 is forbidden vertex');

-- 11 is forbidden
PREPARE v4e3q40 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 5 or id = 11 or id = 13',
    ARRAY[11]::integer[], ARRAY[1]::INTEGER[], 1, true);

PREPARE v4e3q41 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM ( VALUES (1, 'v', 11, ARRAY[12]::bigint[], -1, -1, -1) ) AS t(seq, type, id, contracted_vertices, source, target, cost );
SELECT set_eq('v4e3q40', 'v4e3q41', '5: Directed graph with three edges and 11 is forbidden vertex');

-- 12 is forbidden
PREPARE v4e3q50 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 5 or id = 11 or id = 13',
    ARRAY[12]::integer[], ARRAY[1]::INTEGER[], 1, true);

SELECT set_eq('v4e3q50', 'qempty', '5: Directed graph with three edges and 12 is forbidden vertex');

-- no forbidden vertices
PREPARE v5e3q10 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 2 or id = 3 or id = 12',
    ARRAY[]::integer[], ARRAY[1]::INTEGER[], 1, true);

PREPARE v5e3q11 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM ( VALUES (1, 'v', 4, ARRAY[2, 3]::bigint[], -1, -1, -1), (2, 'v', 10, ARRAY[11]::integer[], -1, -1, -1) ) AS t(seq, type, id, contracted_vertices, source, target, cost );

SELECT set_eq('v5e3q10', 'v5e3q11', '7: Directed graph with three edges and no forbidden vertices');

-- 11 is forbidden
PREPARE v5e3q20 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 2 or id = 3 or id = 12',
    ARRAY[11]::integer[], ARRAY[1]::INTEGER[], 1, true);

PREPARE v5e3q21 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM ( VALUES (1, 'v', 4, ARRAY[2, 3]::bigint[], -1, -1, -1) ) AS t(seq, type, id, contracted_vertices, source, target, cost );

SELECT set_eq('v5e3q20', 'v5e3q21', '7: Directed graph with three edges and 11 is forbidden vertex');

-- 2 is forbidden
PREPARE v5e3q30 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 2 or id = 3 or id = 12',
    ARRAY[2]::integer[], ARRAY[1]::INTEGER[], 1, true);

PREPARE v5e3q31 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM ( VALUES (1, 'v', 10, ARRAY[11]::bigint[], -1, -1, -1) ) AS t(seq, type, id, contracted_vertices, source, target, cost );

SELECT set_eq('v5e3q30', 'v5e3q31', '7: Directed graph with three edges and 2 is forbidden vertex');

-- 3 is forbidden
PREPARE v5e3q40 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 2 or id = 3 or id = 12',
    ARRAY[3]::integer[], ARRAY[1]::INTEGER[], 1, true);

PREPARE v5e3q41 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM ( VALUES (1, 'v', 3, ARRAY[2]::bigint[], -1, -1, -1), (2, 'v', 10, ARRAY[11]::integer[], -1, -1, -1) ) AS t(seq, type, id, contracted_vertices, source, target, cost );

SELECT set_eq('v5e3q40', 'v5e3q41', '7: Directed graph with three edges and 3 is forbidden vertex');

-- 2 and 11 are forbidden
PREPARE v5e3q50 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 2 or id = 3 or id = 12',
    ARRAY[2, 11]::integer[], ARRAY[1]::INTEGER[], 1, true);

SELECT set_eq('v5e3q50', 'qempty', '7: Directed graph with three edges and 2 and 11 are forbidden vertices');

-- 3 and 11 are forbidden
PREPARE v5e3q60 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 2 or id = 3 or id = 12',
    ARRAY[3, 11]::integer[], ARRAY[1]::INTEGER[], 1, true);
PREPARE v5e3q61 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM ( VALUES (1, 'v', 3, ARRAY[2]::bigint[], -1, -1, -1)) AS t(seq, type, id, contracted_vertices, source, target, cost );
SELECT set_eq('v5e3q60', 'v5e3q61', '7: Directed graph with three edges and 3 and 11 are forbidden vertices');

-- no forbidden vertices
PREPARE v6e3q10 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 3 or id = 6',
    ARRAY[]::integer[], ARRAY[1]::INTEGER[], 1, true);

PREPARE v6e3q11 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM ( VALUES (1, 'v', 2, ARRAY[1]::bigint[], -1, -1, -1), (2, 'v', 4, ARRAY[3]::integer[], -1, -1, -1), (3, 'v', 8, ARRAY[7]::integer[], -1, -1, -1) ) AS t(seq, type, id, contracted_vertices, source, target, cost );

SELECT set_eq('v6e3q10', 'v6e3q11', '8: Directed graph with three edges and no forbidden vertices');

-- 1 is forbidden
PREPARE v6e3q20 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 3 or id = 6',
    ARRAY[1]::integer[], ARRAY[1]::INTEGER[], 1, true);

PREPARE v6e3q21 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM ( VALUES (1, 'v', 1, ARRAY[2]::bigint[], -1, -1, -1), (2, 'v', 4, ARRAY[3]::integer[], -1, -1, -1), (3, 'v', 8, ARRAY[7]::integer[], -1, -1, -1)  ) AS t(seq, type, id, contracted_vertices, source, target, cost );

SELECT set_eq('v6e3q20', 'v6e3q21', '8: Directed graph with three edges and 1 is forbidden vertex');

-- 3 is forbidden
PREPARE v6e3q30 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 3 or id = 6',
    ARRAY[3]::integer[], ARRAY[1]::INTEGER[], 1, true);

PREPARE v6e3q31 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM ( VALUES (1, 'v', 2, ARRAY[1]::bigint[], -1, -1, -1), (2, 'v', 8, ARRAY[7]::bigint[], -1, -1, -1) ) AS t(seq, type, id, contracted_vertices, source, target, cost );

SELECT set_eq('v6e3q30', 'v6e3q31', '8: Directed graph with three edges and 3 is forbidden vertex');

-- 7 is forbidden
PREPARE v6e3q40 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 3 or id = 6',
    ARRAY[7]::integer[], ARRAY[1]::INTEGER[], 1, true);

PREPARE v6e3q41 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM ( VALUES (1, 'v', 2, ARRAY[1]::bigint[], -1, -1, -1), (2, 'v', 4, ARRAY[3]::integer[], -1, -1, -1), (3, 'v', 7, ARRAY[8]::integer[], -1, -1, -1)  ) AS t(seq, type, id, contracted_vertices, source, target, cost );

SELECT set_eq('v6e3q40', 'v6e3q41', '8: Directed graph with three edges and 7 is forbidden vertex');

-- 1 and 3 are forbidden
PREPARE v6e3q50 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 3 or id = 6',
    ARRAY[1, 3]::integer[], ARRAY[1]::INTEGER[], 1, true);

PREPARE v6e3q51 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM ( VALUES (1, 'v', 1, ARRAY[2]::bigint[],  -1, -1, -1), (2, 'v', 8, ARRAY[7]::bigint[], -1, -1, -1) ) AS t(seq, type, id, contracted_vertices, source, target, cost );

SELECT set_eq('v6e3q50', 'v6e3q51', '8: Directed graph with three edges and 1 and 3 are forbidden vertices');

-- 3 and 7 are forbidden
PREPARE v6e3q60 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 3 or id = 6',
    ARRAY[3, 7]::integer[], ARRAY[1]::INTEGER[], 1, true);

PREPARE v6e3q61 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM ( VALUES (1, 'v', 2, ARRAY[1]::bigint[], -1, -1, -1), (2, 'v', 7, ARRAY[8]::bigint[], -1, -1, -1) ) AS t(seq, type, id, contracted_vertices, source, target, cost );

SELECT set_eq('v6e3q60', 'v6e3q61', '8: Directed graph with three edges and 3 and 7 are forbidden vertices');

-- 1 and 7 are forbidden
PREPARE v6e3q70 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 3 or id = 6',
    ARRAY[1, 7]::integer[], ARRAY[1]::INTEGER[], 1, true);

PREPARE v6e3q71 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM ( VALUES (1, 'v', 1, ARRAY[2]::bigint[], -1, -1, -1), (2, 'v', 4, ARRAY[3]::bigint[], -1, -1, -1), (3, 'v', 7, ARRAY[8]::bigint[], -1, -1, -1) ) AS t(seq, type, id, contracted_vertices, source, target, cost );

SELECT set_eq('v6e3q70', 'v6e3q71', '8: Directed graph with three edges and 1 and 7 are forbidden vertices');


-- FOUR EDGES
-- no forbidden vertices
PREPARE v4e4q10 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 2 or id = 3 or id = 5 or id = 11',
    ARRAY[]::integer[], ARRAY[1]::INTEGER[], 1, true);

PREPARE v4e4q11 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM ( VALUES (1, 'v', 4, ARRAY[2, 3, 6, 11]::bigint[], -1, -1, -1) ) AS t(seq, type, id, contracted_vertices, source, target, cost );

SELECT set_eq('v4e4q10', 'v4e4q11', '9: Directed graph with four edges and no forbidden vertices');

-- 2 is forbidden
PREPARE v4e4q20 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 2 or id = 3 or id = 5 or id = 11',
    ARRAY[2]::integer[], ARRAY[1]::INTEGER[], 1, true);

PREPARE v4e4q21 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM ( VALUES (1, 'v', 3, ARRAY[6, 11]::bigint[], -1, -1, -1) ) AS t(seq, type, id, contracted_vertices, source, target, cost );

SELECT set_eq('v4e4q20', 'v4e4q21', '9: Directed graph with four edges and 2 is forbidden vertex');

-- 3 is forbidden
PREPARE v4e4q30 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 2 or id = 3 or id = 5 or id = 11',
    ARRAY[3]::integer[], ARRAY[1]::INTEGER[], 1, true);

PREPARE v4e4q31 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM ( VALUES (1, 'v', 3, ARRAY[2, 6, 11]::bigint[], -1, -1, -1)) AS t(seq, type, id, contracted_vertices, source, target, cost );
SELECT set_eq('v4e4q30', 'v4e4q31', '9: Directed graph with four edges and 3 is forbidden vertex');

-- 6 is forbidden
PREPARE v4e4q40 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 2 or id = 3 or id = 5 or id = 11',
    ARRAY[6]::integer[], ARRAY[1]::INTEGER[], 1, true);

PREPARE v4e4q41 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM ( VALUES (1, 'v', 3, ARRAY[2]::bigint[], -1, -1, -1), (2, 'v', 6, ARRAY[11]::bigint[], -1, -1, -1) ) AS t(seq, type, id, contracted_vertices, source, target, cost );
SELECT set_eq('v4e4q40', 'v4e4q41', '9: Directed graph with four edges and 6 is forbidden vertex');

-- 11 is forbidden
PREPARE v4e4q50 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 2 or id = 3 or id = 5 or id = 11',
    ARRAY[11]::integer[], ARRAY[1]::INTEGER[], 1, true);

PREPARE v4e4q51 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM ( VALUES (1, 'v', 3, ARRAY[2]::bigint[], -1, -1, -1) ) AS t(seq, type, id, contracted_vertices, source, target, cost );
SELECT set_eq('v4e4q50', 'v4e4q51', '9: Directed graph with four edges and 11 is forbidden vertex');


-- 2,3 are forbidden
PREPARE v4e4q60 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 2 or id = 3 or id = 5 or id = 11',
    ARRAY[2, 3]::integer[], ARRAY[1]::INTEGER[], 1, true);

PREPARE v4e4q61 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM ( VALUES (1, 'v', 3, ARRAY[6, 11]::bigint[], -1, -1, -1) ) AS t(seq, type, id, contracted_vertices, source, target, cost );

SELECT set_eq('v4e4q60', 'v4e4q61', '9: Directed graph with four edges and 2 and 3 is forbidden vertex');


-- 2,6 are forbidden
PREPARE v4e4q70 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 2 or id = 3 or id = 5 or id = 11',
    ARRAY[2, 6]::integer[], ARRAY[1]::INTEGER[], 1, true);

PREPARE v4e4q71 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM ( VALUES (1, 'v', 6, ARRAY[11]::bigint[], -1, -1, -1) ) AS t(seq, type, id, contracted_vertices, source, target, cost );

SELECT set_eq('v4e4q70', 'v4e4q71', '9: Directed graph with four edges and 2 and 6 forbidden vertex');

-- 2,11 are forbidden
PREPARE v4e4q80 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 2 or id = 3 or id = 5 or id = 11',
    ARRAY[2, 11]::integer[], ARRAY[1]::INTEGER[], 1, true);

SELECT set_eq('v4e4q80', 'qempty', '9: Directed graph with four edges and 2 and 11 forbidden vertex');

-- 3,6 are forbidden
PREPARE v4e4q90 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 2 or id = 3 or id = 5 or id = 11',
    ARRAY[3, 6]::integer[], ARRAY[1]::INTEGER[], 1, true);

PREPARE v4e4q91 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM ( VALUES (1, 'v', 3, ARRAY[2]::bigint[], -1, -1, -1), (2, 'v', 6, ARRAY[11]::bigint[], -1, -1, -1) ) AS t(seq, type, id, contracted_vertices, source, target, cost );

SELECT set_eq('v4e4q90', 'v4e4q91', '9: Directed graph with four edges and 3 and 6 forbidden vertex');

-- 3,11 are forbidden
PREPARE v4e4q100 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 2 or id = 3 or id = 5 or id = 11',
    ARRAY[3, 11]::integer[], ARRAY[1]::INTEGER[], 1, true);

PREPARE v4e4q101 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM ( VALUES (1, 'v', 3, ARRAY[2]::bigint[], -1, -1, -1) ) AS t(seq, type, id, contracted_vertices, source, target, cost );

SELECT set_eq('v4e4q100', 'v4e4q101', '9: Directed graph with four edges and 3 and 11 forbidden vertex');

-- 6,11 are forbidden
PREPARE v4e4q110 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 2 or id = 3 or id = 5 or id = 11',
    ARRAY[6, 11]::integer[], ARRAY[1]::INTEGER[], 1, true);

PREPARE v4e4q111 AS
SELECT seq, type, id, unnest(contracted_vertices) AS contracted_vertices, source, target, cost  FROM ( VALUES (1, 'v', 3, ARRAY[2]::bigint[], -1, -1, -1) ) AS t(seq, type, id, contracted_vertices, source, target, cost );

SELECT set_eq('v4e4q110', 'v4e4q111', '9: Directed graph with four edges and 6 and 11 forbidden vertex');


SELECT finish();
ROLLBACK;