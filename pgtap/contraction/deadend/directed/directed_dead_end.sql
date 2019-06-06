\i setup.sql

SELECT plan(39);

SET client_min_messages TO WARNING;
-- TESTING ONE CYCLE OF DEAD END CONTRACTION FOR A DIRECTED GRAPH

-- GRAPH: 1 <=> 2
PREPARE graph_e_1 AS
SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1;

-- GRAPH: 4 -> 3 -> 2
PREPARE graph_e_2_3 AS
SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id IN (2, 3);

-- GRAPH: 6 -> 11 <- 10
PREPARE graph_e_11_12 AS
SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id IN (11, 12);

-- GRAPH: 3 -> 2   10 -> 11
PREPARE graph_e_2_12 AS
SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id IN (2, 12);

-- GRAPH: 3 -> 6 -> 11 -> 12
PREPARE graph_e_5_11_13 AS
SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id IN (5, 11, 13);

-- GRAPH: 4-> 3 -> 2   10 -> 11
PREPARE graph_e_2_3_12 AS
SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id IN (2, 3, 12);

-- GRAPH: 1 <=> 2  4 -> 3  7 <=> 8
PREPARE graph_e_1_3_6 AS
SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id IN (1, 3, 6);

-- GRAPH: 4 -> 3 -> 2   3 -> 6 -> 11
PREPARE graph_e_2_3_5_11 AS
SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id IN (2, 3, 5, 11);

-- SINGLE EDGE
-- GRAPH: 1 <=> 2
-- no forbidden vertices
PREPARE v2e1q10 AS
SELECT * FROM pgr_contraction(
    'graph_e_1',
    ARRAY[1]::integer[], 1, ARRAY[]::integer[], true);

PREPARE v2e1q11 AS
SELECT *
FROM (VALUES ('v', 2, ARRAY[1]::BIGINT[], -1, -1, -1) ) AS t(type, id, contracted_vertices, source, target, cost ) ;

SELECT set_eq('v2e1q10', 'v2e1q11', 'graph_e_1 QUERY 1: Directed graph with single edge and no forbidden vertices');

-- GRAPH: 1 <=> 2
-- 1 is forbidden vertex

PREPARE v2e1q20 AS
SELECT * FROM pgr_contraction(
    'graph_e_1',
    ARRAY[1]::integer[], 1, ARRAY[1]::INTEGER[], true);

PREPARE v2e1q21 AS
SELECT *
FROM ( VALUES ('v', 1, ARRAY[2]::BIGINT[], -1, -1, -1) ) AS t(type, id, contracted_vertices, source, target, cost ) ;


SELECT set_eq('v2e1q20', 'v2e1q21', 'graph_e_1 QUERY 2: Directed graph with single edge and 1 as forbidden vertex');


-- TWO QUERY
-- GRAPH: 4 -> 3 -> 2
-- no forbidden vertices
PREPARE v3e2q10 AS
SELECT * FROM pgr_contraction(
    'graph_e_2_3',
    ARRAY[1]::integer[], 1, ARRAY[]::integer[], true);

PREPARE v3e2q11 AS
SELECT *
FROM ( VALUES ('v', 4, ARRAY[2, 3]::BIGINT[], -1, -1, -1) ) AS t(type, id, contracted_vertices, source, target, cost ) ;

SELECT set_eq('v3e2q10', 'v3e2q11', 'graph_e_2_3 QUERY 1: Directed graph two edges and no forbidden vertices');

-- GRAPH: 4 -> 3 -> 2
-- 3 is forbidden
PREPARE v3e2q20 AS
SELECT * FROM pgr_contraction(
    'graph_e_2_3',
    ARRAY[1]::INTEGER[], 1, ARRAY[3]::integer[], true);

PREPARE v3e2q21 AS
SELECT type, id, contracted_vertices, source, target, cost
FROM ( VALUES ('v', 3, ARRAY[2,4]::BIGINT[], -1, -1, -1) ) AS t(type, id, contracted_vertices, source, target, cost ) ;

SELECT set_eq('v3e2q20', 'v3e2q21', 'graph_e_2_3 QUERY 2: Directed graph with two edges and vertex 3 as forbidden vertex');

-- GRAPH: 4 -> 3 -> 2
-- 2 is forbidden
PREPARE v3e2q30 AS
SELECT * FROM pgr_contraction(
    'graph_e_2_3',
    ARRAY[1]::INTEGER[], 1, ARRAY[2]::integer[], true);

PREPARE v3e2q31 AS
SELECT *
FROM ( VALUES ('v', 2, ARRAY[3,4]::BIGINT[], -1, -1, -1) ) AS t(type, id, contracted_vertices, source, target, cost ) ;

SELECT set_eq('v3e2q30', 'v3e2q31', 'graph_e_2_3 QUERY 3: Directed graph with two edges and vertex 2 as forbidden vertex');

-- GRAPH: 6 -> 11 <- 10
-- no forbidden vertices
PREPARE v3e2q40 AS
SELECT * FROM pgr_contraction(
    'graph_e_11_12',
    ARRAY[1]::integer[], 1, ARRAY[]::integer[], true);

PREPARE v3e2q41 AS
SELECT *
FROM (VALUES ('v', 10, ARRAY[6,11]::BIGINT[], -1, -1, -1) ) AS t(type, id, contracted_vertices, source, target, cost ) ;

SELECT set_eq('v3e2q40', 'v3e2q41', 'graph_e_11_12 QUERY 1: Directed graph with two edges and no forbidden vertices');


-- GRAPH: 6 -> 11 <- 10
-- 11 is forbidden
PREPARE v3e2q50 AS
SELECT * FROM pgr_contraction(
    'graph_e_11_12',
    ARRAY[1]::INTEGER[], 1, ARRAY[11]::integer[], true);

PREPARE v3e2q51 AS
SELECT *
FROM (VALUES ('v', 11, ARRAY[6,10]::BIGINT[], -1, -1, -1) ) AS t(type, id, contracted_vertices, source, target, cost ) ;

SELECT set_eq('v3e2q50', 'v3e2q51', 'graph_e_11_12 QUERY 2: Directed graph with two edges and no forbidden vertices');

-- GRAPH: 3 -> 2   10 <=> 11
-- no forbidden
PREPARE v3e2q60 AS
SELECT * FROM pgr_contraction(
    'graph_e_2_12',
    ARRAY[1]::integer[], 1, ARRAY[]::integer[], true);

PREPARE v3e2q61 AS
SELECT *
FROM (VALUES
    ('v', 3, ARRAY[2]::BIGINT[], -1, -1, -1),
    ('v', 11, ARRAY[10]::integer[], -1, -1, -1)
) AS t(type, id, contracted_vertices, source, target, cost );

SELECT set_eq('v3e2q60', 'v3e2q61', 'graph_e_2_12 QUERY 1: Directed graph with two edges and no forbidden vertices');

-- GRAPH: 3 -> 2   10 <=> 11
-- 2 is forbidden
PREPARE v3e2q70 AS
SELECT * FROM pgr_contraction(
    'graph_e_2_12',
    ARRAY[1]::INTEGER[], 1, ARRAY[2]::integer[], true);

PREPARE v3e2q71 AS
SELECT *
FROM (VALUES
    ('v', 2, ARRAY[3]::BIGINT[], -1, -1, -1),
    ('v', 11, ARRAY[10]::BIGINT[], -1, -1, -1)
) AS t(type, id, contracted_vertices, source, target, cost );

SELECT set_eq('v3e2q70', 'v3e2q71', 'graph_e_2_12 QUERY 2: Directed graph with two edges and 2 is forbidden vertex');

-- GRAPH: 3 -> 2   10 <=> 11
-- 11 is forbidden
PREPARE v3e2q80 AS
SELECT * FROM pgr_contraction(
    'graph_e_2_12',
    ARRAY[1]::INTEGER[], 1, ARRAY[11]::integer[], true);

PREPARE v3e2q81 AS
SELECT *
FROM (VALUES
    ('v', 3, ARRAY[2]::BIGINT[], -1, -1, -1),
    ('v', 11, ARRAY[10]::BIGINT[], -1, -1, -1)
) AS t(type, id, contracted_vertices, source, target, cost );

SELECT set_eq('v3e2q80', 'v3e2q81', 'graph_e_2_12 QUERY 3: Directed graph with two edges and 11 is forbidden vertex');


-- THREE QUERY
-- GRAPH: 3 -> 6 -> 11 -> 12
-- no forbidden vertices
PREPARE v4e3q10 AS
SELECT * FROM pgr_contraction(
    'graph_e_5_11_13',
    ARRAY[1]::integer[], 1, ARRAY[]::integer[], true);

PREPARE v4e3q11 AS
SELECT *
FROM (VALUES
    ('v', 12, ARRAY[3, 6, 11]::BIGINT[], -1, -1, -1)
) AS t(type, id, contracted_vertices, source, target, cost );

SELECT set_eq('v4e3q10', 'v4e3q11', 'graph_e_5_11_13 QUERY 1: Directed graph with three edges and no forbidden vertices');

-- GRAPH: 3 -> 6 -> 11 -> 12
-- 3 is forbidden
PREPARE v4e3q20 AS
SELECT * FROM pgr_contraction(
    'graph_e_5_11_13',
    ARRAY[1]::INTEGER[], 1, ARRAY[3]::integer[], true);

PREPARE v4e3q21 AS
SELECT *
FROM (VALUES
    ('v', 3, ARRAY[6, 11, 12]::BIGINT[], -1, -1, -1)
) AS t(type, id, contracted_vertices, source, target, cost );

SELECT set_eq('v4e3q20', 'v4e3q21', 'graph_e_5_11_13 QUERY 2: Directed graph with three edges and 3 is forbidden vertex');


-- GRAPH: 3 -> 6 -> 11 -> 12
-- 6 is forbidden
PREPARE v4e3q30 AS
SELECT * FROM pgr_contraction(
    'graph_e_5_11_13',
    ARRAY[1]::INTEGER[], 1, ARRAY[6]::integer[], true);

PREPARE v4e3q31 AS
SELECT *
FROM (VALUES
    ('v', 6, ARRAY[3, 11, 12]::BIGINT[], -1, -1, -1)
) AS t(type, id, contracted_vertices, source, target, cost );

SELECT set_eq('v4e3q30', 'v4e3q31', 'graph_e_5_11_13 QUERY 3: Directed graph with three edges and 6 is forbidden vertex');


-- GRAPH: 3 -> 6 -> 11 -> 12
-- 11 is forbidden
PREPARE v4e3q40 AS
SELECT * FROM pgr_contraction(
    'graph_e_5_11_13',
    ARRAY[1]::INTEGER[], 1, ARRAY[11]::integer[], true);

PREPARE v4e3q41 AS
SELECT *
FROM (VALUES
    ('v', 11, ARRAY[3,6,12]::BIGINT[], -1, -1, -1)
) AS t(type, id, contracted_vertices, source, target, cost );
SELECT set_eq('v4e3q40', 'v4e3q41', 'graph_e_5_11_13 QUERY 4: Directed graph with three edges and 11 is forbidden vertex');

-- GRAPH: 3 -> 6 -> 11 -> 12
-- forbidden: 3, 6, 12
PREPARE v4e3q50 AS
SELECT * FROM pgr_contraction(
    'graph_e_5_11_13',
    ARRAY[1]::INTEGER[], 1, ARRAY[3,6,12]::integer[], true);

SELECT is_empty('v4e3q50', 'graph_e_5_11_13 QUERY 5: Directed graph with three edges and 12 is forbidden vertex');

-- GRAPH: 4-> 3 -> 2   10 -> 11
-- no forbidden vertices
PREPARE v5e3q10 AS
SELECT * FROM pgr_contraction(
    'graph_e_2_3_12',
    ARRAY[1]::integer[], 1, ARRAY[]::integer[], true);

PREPARE v5e3q11 AS
SELECT *
FROM (VALUES
    ('v', 4, ARRAY[2, 3]::BIGINT[], -1, -1, -1),
    ('v', 11, ARRAY[10], -1, -1, -1)
) AS t(type, id, contracted_vertices, source, target, cost );

SELECT set_eq('v5e3q10', 'v5e3q11', 'graph_e_2_3_12 QUERY 1: Directed graph with three edges and no forbidden vertices');

-- GRAPH: 4-> 3 -> 2   10 -> 11
-- 11 is forbidden
PREPARE v5e3q20 AS
SELECT * FROM pgr_contraction(
    'graph_e_2_3_12',
    ARRAY[1]::INTEGER[], 1, ARRAY[11]::integer[], true);

PREPARE v5e3q21 AS
SELECT *
FROM (VALUES
    ('v', 4, ARRAY[2, 3]::BIGINT[], -1, -1, -1),
    ('v', 11, ARRAY[10], -1, -1, -1)
) AS t(type, id, contracted_vertices, source, target, cost );

SELECT set_eq('v5e3q20', 'v5e3q21', 'graph_e_2_3_12 QUERY 2: Directed graph with three edges and 11 is forbidden vertex');

-- GRAPH: 4-> 3 -> 2   10 -> 11
-- 2 is forbidden
PREPARE v5e3q30 AS
SELECT * FROM pgr_contraction(
    'graph_e_2_3_12',
    ARRAY[1]::INTEGER[], 1, ARRAY[2]::integer[], true);

PREPARE v5e3q31 AS
SELECT *
FROM (VALUES
    ('v', 2, ARRAY[3,4]::BIGINT[], -1, -1, -1),
    ('v', 11, ARRAY[10], -1, -1, -1)
) AS t(type, id, contracted_vertices, source, target, cost );

SELECT set_eq('v5e3q30', 'v5e3q31', 'graph_e_2_3_12 QUERY 3: Directed graph with three edges and 2 is forbidden vertex');

-- GRAPH: 4-> 3 -> 2   10 -> 11
-- 3 is forbidden
PREPARE v5e3q40 AS
SELECT * FROM pgr_contraction(
    'graph_e_2_3_12',
    ARRAY[1]::INTEGER[], 1, ARRAY[3]::integer[], true);

PREPARE v5e3q41 AS
SELECT *
FROM (VALUES
    ('v', 3, ARRAY[2,4]::BIGINT[], -1, -1, -1),
    ('v', 11, ARRAY[10], -1, -1, -1)
) AS t(type, id, contracted_vertices, source, target, cost);

SELECT set_eq('v5e3q40', 'v5e3q41', 'graph_e_2_3_12 QUERY 4: Directed graph with three edges and 3 is forbidden vertex');

-- GRAPH: 4-> 3 -> 2   10 -> 11
-- 2 and 11 are forbidden
PREPARE v5e3q50 AS
SELECT * FROM pgr_contraction(
    'graph_e_2_3_12',
    ARRAY[1]::INTEGER[], 1, ARRAY[2, 11]::integer[], true);

PREPARE v5e3q51 AS
SELECT *
FROM (VALUES
    ('v', 2, ARRAY[3,4]::BIGINT[], -1, -1, -1),
    ('v', 11, ARRAY[10], -1, -1, -1)
) AS t(type, id, contracted_vertices, source, target, cost);

SELECT set_eq('v5e3q50', 'v5e3q51', 'graph_e_2_3_12 QUERY 5: Directed graph with three edges and 2 and 11 are forbidden vertices');

-- GRAPH: 4-> 3 -> 2   10 -> 11
-- 3 and 11 are forbidden
PREPARE v5e3q60 AS
SELECT * FROM pgr_contraction(
    'graph_e_2_3_12',
    ARRAY[1]::INTEGER[], 1, ARRAY[3, 11]::integer[], true);

PREPARE v5e3q61 AS
SELECT *
FROM (VALUES
    ('v', 3, ARRAY[2,4]::BIGINT[], -1, -1, -1),
    ('v', 11, ARRAY[10], -1, -1, -1)
) AS t(type, id, contracted_vertices, source, target, cost );

SELECT set_eq('v5e3q60', 'v5e3q61', 'graph_e_2_3_12 QUERY 6: Directed graph with three edges and 3 and 11 are forbidden vertices');

-- GRAPH: 1 <=> 2  4 -> 3  7 <=> 8
-- no forbidden vertices
PREPARE v6e3q10 AS
SELECT * FROM pgr_contraction(
    'graph_e_1_3_6',
    ARRAY[1]::integer[], 1, ARRAY[]::integer[], true);

PREPARE v6e3q11 AS
SELECT *
FROM (VALUES
    ('v', 2, ARRAY[1]::BIGINT[], -1, -1, -1),
    ('v', 4, ARRAY[3]::integer[], -1, -1, -1),
    ('v', 8, ARRAY[7]::integer[], -1, -1, -1)
) AS t(type, id, contracted_vertices, source, target, cost );

SELECT set_eq('v6e3q10', 'v6e3q11', 'graph_e_1_3_6 QUERY 1: Directed graph with three edges and no forbidden vertices');

-- GRAPH: 1 <=> 2  4 -> 3  7 <=> 8
-- 1 is forbidden
PREPARE v6e3q20 AS
SELECT * FROM pgr_contraction(
    'graph_e_1_3_6',
    ARRAY[1]::INTEGER[], 1, ARRAY[1]::integer[], true);

PREPARE v6e3q21 AS
SELECT *
FROM (VALUES
    ('v', 1, ARRAY[2]::BIGINT[], -1, -1, -1),
    ('v', 4, ARRAY[3]::integer[], -1, -1, -1),
    ('v', 8, ARRAY[7]::integer[], -1, -1, -1)
) AS t(type, id, contracted_vertices, source, target, cost );

SELECT set_eq('v6e3q20', 'v6e3q21', 'graph_e_1_3_6 QUERY 2: Directed graph with three edges and 1 is forbidden vertex');

-- GRAPH: 1 <=> 2  4 -> 3  7 <=> 8
-- 3 is forbidden
PREPARE v6e3q30 AS
SELECT * FROM pgr_contraction(
    'graph_e_1_3_6',
    ARRAY[1]::INTEGER[], 1, ARRAY[3]::integer[], true);

PREPARE v6e3q31 AS
SELECT *
FROM (VALUES
    ('v', 2, ARRAY[1]::BIGINT[], -1, -1, -1),
    ('v', 3, ARRAY[4]::BIGINT[], -1, -1, -1),
    ('v', 8, ARRAY[7]::BIGINT[], -1, -1, -1)
) AS t(type, id, contracted_vertices, source, target, cost );

SELECT set_eq('v6e3q30', 'v6e3q31', 'graph_e_1_3_6 QUERY 3: Directed graph with three edges and 3 is forbidden vertex');

-- GRAPH: 1 <=> 2  4 -> 3  7 <=> 8
-- 7 is forbidden
PREPARE v6e3q40 AS
SELECT * FROM pgr_contraction(
    'graph_e_1_3_6',
    ARRAY[1]::INTEGER[], 1, ARRAY[7]::integer[], true);

PREPARE v6e3q41 AS
SELECT *
FROM (VALUES
    ('v', 2, ARRAY[1]::BIGINT[], -1, -1, -1),
    ('v', 4, ARRAY[3]::integer[], -1, -1, -1),
    ('v', 7, ARRAY[8]::integer[], -1, -1, -1)
) AS t(type, id, contracted_vertices, source, target, cost );

SELECT set_eq('v6e3q40', 'v6e3q41', 'graph_e_1_3_6 QUERY 4: Directed graph with three edges and 7 is forbidden vertex');

-- GRAPH: 1 <=> 2  4 -> 3  7 <=> 8
-- 1 and 3 are forbidden
PREPARE v6e3q50 AS
SELECT * FROM pgr_contraction(
    'graph_e_1_3_6',
    ARRAY[1]::INTEGER[], 1, ARRAY[1, 3]::integer[], true);

PREPARE v6e3q51 AS
SELECT *
FROM (VALUES
    ('v', 1, ARRAY[2]::BIGINT[], -1, -1, -1),
    ('v', 3, ARRAY[4]::BIGINT[], -1, -1, -1),
    ('v', 8, ARRAY[7]::BIGINT[], -1, -1, -1)
) AS t(type, id, contracted_vertices, source, target, cost );

SELECT set_eq('v6e3q50', 'v6e3q51', 'graph_e_1_3_6 QUERY 5: Directed graph with three edges and 1 and 3 are forbidden vertices');

-- GRAPH: 1 <=> 2  4 -> 3  7 <=> 8
-- 3 and 7 are forbidden
PREPARE v6e3q60 AS
SELECT * FROM pgr_contraction(
    'graph_e_1_3_6',
    ARRAY[1]::INTEGER[], 1, ARRAY[3, 7]::integer[], true);

PREPARE v6e3q61 AS
SELECT *
FROM (VALUES
    ('v', 2, ARRAY[1]::BIGINT[], -1, -1, -1),
    ('v', 3, ARRAY[4]::BIGINT[], -1, -1, -1),
    ('v', 7, ARRAY[8]::BIGINT[], -1, -1, -1)
) AS t(type, id, contracted_vertices, source, target, cost );

SELECT set_eq('v6e3q60', 'v6e3q61', 'graph_e_1_3_6 QUERY 6: Directed graph with three edges and 3 and 7 are forbidden vertices');

-- GRAPH: 1 <=> 2  4 -> 3  7 <=> 8
-- 1 and 7 are forbidden
PREPARE v6e3q70 AS
SELECT * FROM pgr_contraction(
    'graph_e_1_3_6',
    ARRAY[1]::INTEGER[], 1, ARRAY[1, 7]::integer[], true);

PREPARE v6e3q71 AS
SELECT *
FROM (VALUES
    ('v', 1, ARRAY[2]::BIGINT[], -1, -1, -1),
    ('v', 4, ARRAY[3]::BIGINT[], -1, -1, -1),
    ('v', 7, ARRAY[8]::BIGINT[], -1, -1, -1)
) AS t(type, id, contracted_vertices, source, target, cost );

SELECT set_eq('v6e3q70', 'v6e3q71', 'graph_e_1_3_6 QUERY 7: Directed graph with three edges and 1 and 7 are forbidden vertices');


-- FOUR EDGES
-- GRAPH: 4 -> 3 -> 2   3 -> 6 -> 11
-- no forbidden vertices
PREPARE v4e4q10 AS
SELECT * FROM pgr_contraction(
    'graph_e_2_3_5_11',
    ARRAY[1]::integer[], 1, ARRAY[]::integer[], true);

PREPARE v4e4q11 AS
SELECT *
FROM (VALUES
    ('v', 11, ARRAY[2, 3, 4, 6]::BIGINT[], -1, -1, -1)
) AS t(type, id, contracted_vertices, source, target, cost );

SELECT set_eq('v4e4q10', 'v4e4q11', 'graph_e_2_3_5_11 QUERY 1: Directed graph with four edges and no forbidden vertices');

-- GRAPH: 4 -> 3 -> 2   3 -> 6 -> 11
-- 2 is forbidden
PREPARE v4e4q20 AS
SELECT * FROM pgr_contraction(
    'graph_e_2_3_5_11',
    ARRAY[1]::INTEGER[], 1, ARRAY[2]::integer[], true);

PREPARE v4e4q21 AS
SELECT *
FROM (VALUES
    ('v', 2, ARRAY[3, 4, 6, 11]::BIGINT[], -1, -1, -1)
) AS t(type, id, contracted_vertices, source, target, cost );

SELECT set_eq('v4e4q20', 'v4e4q21', 'graph_e_2_3_5_11 QUERY 2: Directed graph with four edges and 2 is forbidden vertex');

-- GRAPH: 4 -> 3 -> 2   3 -> 6 -> 11
-- 3 is forbidden
PREPARE v4e4q30 AS
SELECT * FROM pgr_contraction(
    'graph_e_2_3_5_11',
    ARRAY[1]::INTEGER[], 1, ARRAY[3]::integer[], true);

PREPARE v4e4q31 AS
SELECT *
FROM (VALUES
    ('v', 3, ARRAY[2, 4, 6, 11]::BIGINT[], -1, -1, -1)
) AS t(type, id, contracted_vertices, source, target, cost );

SELECT set_eq('v4e4q30', 'v4e4q31', 'FOUR QUERY 3: Directed graph with four edges and 3 is forbidden vertex');

-- GRAPH: 4 -> 3 -> 2   3 -> 6 -> 11
-- 6 is forbidden
PREPARE v4e4q40 AS
SELECT * FROM pgr_contraction(
    'graph_e_2_3_5_11',
    ARRAY[1]::INTEGER[], 1, ARRAY[6]::integer[], true);

PREPARE v4e4q41 AS
SELECT *
FROM (VALUES
    ('v', 6, ARRAY[2,3,4,11]::BIGINT[], -1, -1, -1)
) AS t(type, id, contracted_vertices, source, target, cost );

SELECT set_eq('v4e4q40', 'v4e4q41', 'graph_e_2_3_5_11 QUERY 4: Directed graph with four edges and 6 is forbidden vertex');

-- GRAPH: 4 -> 3 -> 2   3 -> 6 -> 11
-- 11 is forbidden
PREPARE v4e4q50 AS
SELECT * FROM pgr_contraction(
    'graph_e_2_3_5_11',
    ARRAY[1]::INTEGER[], 1, ARRAY[11]::integer[], true);

PREPARE v4e4q51 AS
SELECT *
FROM (VALUES
    ('v', 11, ARRAY[2,3,4,6]::BIGINT[], -1, -1, -1)
) AS t(type, id, contracted_vertices, source, target, cost );

SELECT set_eq('v4e4q50', 'v4e4q51', 'graph_e_2_3_5_11 QUERY 5: Directed graph with four edges and 11 is forbidden vertex');


-- GRAPH: 4 -> 3 -> 2   3 -> 6 -> 11
-- 2,3 are forbidden
PREPARE v4e4q60 AS
SELECT * FROM pgr_contraction(
    'graph_e_2_3_5_11',
    ARRAY[1]::INTEGER[], 1, ARRAY[2, 3]::integer[], true);

PREPARE v4e4q61 AS
SELECT *
FROM (VALUES
    ('v', 3, ARRAY[4, 6, 11]::BIGINT[], -1, -1, -1)
) AS t(type, id, contracted_vertices, source, target, cost );

SELECT set_eq('v4e4q60', 'v4e4q61', 'graph_e_2_3_5_11 QUERY 6: Directed graph with four edges and 2 and 3 is forbidden vertex');


-- GRAPH: 4 -> 3 -> 2   3 -> 6 -> 11
-- 2,6 are forbidden
PREPARE v4e4q70 AS
SELECT * FROM pgr_contraction(
    'graph_e_2_3_5_11',
    ARRAY[1]::INTEGER[], 1, ARRAY[2, 6]::integer[], true);

PREPARE v4e4q71 AS
SELECT *
FROM (VALUES
    ('v', 3, ARRAY[4]::BIGINT[], -1, -1, -1),
    ('v', 6, ARRAY[11]::BIGINT[], -1, -1, -1)
) AS t(type, id, contracted_vertices, source, target, cost );

SELECT set_eq('v4e4q70', 'v4e4q71', 'graph_e_2_3_5_11 QUERY 7: Directed graph with four edges and 2 and 6 forbidden vertex');

-- GRAPH: 4 -> 3 -> 2   3 -> 6 -> 11
-- 2,11 are forbidden
PREPARE v4e4q80 AS
SELECT * FROM pgr_contraction(
    'graph_e_2_3_5_11',
    ARRAY[1]::INTEGER[], 1, ARRAY[2, 11]::integer[], true);

PREPARE v4e4q81 AS
SELECT *
FROM (VALUES
    ('v', 3, ARRAY[4]::BIGINT[], -1, -1, -1)
) AS t(type, id, contracted_vertices, source, target, cost );

SELECT set_eq('v4e4q80', 'v4e4q81', 'graph_e_2_3_5_11 QUERY 8: Directed graph with four edges and 2 and 11 forbidden vertex');

-- GRAPH: 4 -> 3 -> 2   3 -> 6 -> 11
-- 3,6 are forbidden
PREPARE v4e4q90 AS
SELECT * FROM pgr_contraction(
    'graph_e_2_3_5_11',
    ARRAY[1]::INTEGER[], 1, ARRAY[3, 6]::integer[], true);

PREPARE v4e4q91 AS
SELECT *
FROM (VALUES
    ('v', 3, ARRAY[2,4]::BIGINT[], -1, -1, -1),
    ('v', 6, ARRAY[11]::BIGINT[], -1, -1, -1)
) AS t(type, id, contracted_vertices, source, target, cost );

SELECT set_eq('v4e4q90', 'v4e4q91', 'graph_e_2_3_5_11 QUERY 9: Directed graph with four edges and 3 and 6 forbidden vertex');

-- GRAPH: 4 -> 3 -> 2   3 -> 6 -> 11
-- 3,11 are forbidden
PREPARE v4e4q100 AS
SELECT * FROM pgr_contraction(
    'graph_e_2_3_5_11',
    ARRAY[1]::INTEGER[], 1, ARRAY[3, 11]::integer[], true);

PREPARE v4e4q101 AS
SELECT *
FROM (VALUES
    ('v', 3, ARRAY[2,4]::BIGINT[], -1, -1, -1)
) AS t(type, id, contracted_vertices, source, target, cost );

SELECT set_eq('v4e4q100', 'v4e4q101', 'graph_e_2_3_5_11 QUERY 10: Directed graph with four edges and 3 and 11 forbidden vertex');

-- GRAPH: 4 -> 3 -> 2   3 -> 6 -> 11
-- 6,11 are forbidden
PREPARE v4e4q110 AS
SELECT * FROM pgr_contraction(
    'graph_e_2_3_5_11',
    ARRAY[1]::INTEGER[], 1, ARRAY[6, 11]::integer[], true);

PREPARE v4e4q111 AS
SELECT *
FROM (VALUES
    ('v', 6, ARRAY[2,3,4]::BIGINT[], -1, -1, -1)
) AS t(type, id, contracted_vertices, source, target, cost );

SELECT set_eq('v4e4q110', 'v4e4q111', 'graph_e_2_3_5_11 QUERY 11: Directed graph with four edges and 6 and 11 forbidden vertex');

SELECT finish();
ROLLBACK;
