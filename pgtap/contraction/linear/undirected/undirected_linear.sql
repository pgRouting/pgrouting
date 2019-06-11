\i setup.sql

SELECT plan(26);

-- TESTING ONE CYCLE OF LINEAR CONTRACTION FOR A DIRECTED GRAPH

-- GRAPH 1 - 2 - 3
PREPARE graph_e_1_2 AS
    SELECT id, source, target, cost, reverse_cost
FROM edge_table WHERE id IN (1, 2);

-- GRAPH 1 - 2 - 5
PREPARE graph_e_1_4 AS
    SELECT id, source, target, cost, reverse_cost
FROM edge_table WHERE id IN (1, 4);

-- GRAPH 6 - 11 - 10
PREPARE graph_e_11_12 AS
    SELECT id, source, target, cost, reverse_cost
FROM edge_table WHERE id IN (11, 12);

-- GRAPH 4 - 3 - 6 - 11
PREPARE graph_e_3_5_11 AS
    SELECT id, source, target, cost, reverse_cost
FROM edge_table WHERE id IN (3, 5, 11);

-- GRAPH 6 - 11 - 12 - 9 - 6
PREPARE graph_e_9_11_13_15 AS
    SELECT id, source, target, cost, reverse_cost
FROM edge_table WHERE id IN (9, 11, 13, 15);


-- TWO EDGES

-- GRAPH 1 - 2 - 3
-- no forbidden vertices
PREPARE graph_e_1_2_q1 AS
SELECT type, id, contracted_vertices, source, target, cost
FROM pgr_contraction(
    'graph_e_1_2',
    ARRAY[2]::INTEGER[], 1, ARRAY[]::INTEGER[], false);

PREPARE graph_e_1_2_sol1 AS
SELECT type, id, contracted_vertices, source, target, cost
FROM (VALUES
    ('e', -1, ARRAY[2]::BIGINT[], 1, 3, 2))
AS t(type, id, contracted_vertices, source, target, cost);

SELECT set_eq('graph_e_1_2_q1', 'graph_e_1_2_sol1', 'graph_e_1_2 QUERY 1: no forbidden vertices');

-- GRAPH 1 - 2 - 3
-- 2 is forbidden
PREPARE graph_e_1_2_q2 AS
SELECT type, id, contracted_vertices, source, target, cost
FROM pgr_contraction(
    'graph_e_1_2',
    ARRAY[2]::INTEGER[], 1, ARRAY[2]::INTEGER[], false);

SELECT is_empty('graph_e_1_2_q2', 'graph_e_1_2 QUERY 2: Directed graph with two edges and 2 is forbidden vertex');

--------------------
-- GRAPH 1 - 2 - 5
PREPARE graph_e_1_4_q1 AS
SELECT type, id, contracted_vertices, source, target, cost
FROM pgr_contraction(
    'graph_e_1_4',
    ARRAY[2]::INTEGER[], 1, ARRAY[]::INTEGER[], false);

PREPARE graph_e_1_4_sol1 AS
SELECT type, id, contracted_vertices, source, target, cost
FROM (VALUES
    ('e', -1, ARRAY[2]::BIGINT[], 1, 5, 2))
AS t(type, id, contracted_vertices, source, target, cost);

SELECT set_eq('graph_e_1_4_q1', 'graph_e_1_4_sol1', 'graph_e_1_4 QUERY 1: Directed graph with two edges and no forbidden vertices');

-- GRAPH 1 - 2 - 5
-- 2 is forbidden
PREPARE graph_e_1_4_q2 AS
SELECT type, id, contracted_vertices, source, target, cost
FROM pgr_contraction(
    'graph_e_1_4',
    ARRAY[2]::INTEGER[], 1, ARRAY[2]::INTEGER[], false);

SELECT is_empty('graph_e_1_4_q2', 'graph_e_1_4 QUERY 2: Directed graph with two edges and 2 is forbidden vertex');


--------------------
-- GRAPH 6 - 11 - 10
-- no forbidden vertices
PREPARE graph_e_11_12_q1 AS
SELECT type, id, contracted_vertices, source, target, cost
FROM pgr_contraction(
    'graph_e_11_12',
    ARRAY[2]::INTEGER[], 1, ARRAY[]::INTEGER[], false);

PREPARE graph_e_11_12_sol1 AS
SELECT type, id, contracted_vertices, source, target, cost
FROM (VALUES
    ('e', -1, ARRAY[11]::BIGINT[], 6, 10, 2))
AS t(type, id, contracted_vertices, source, target, cost);

SELECT set_eq('graph_e_11_12_q1', 'graph_e_11_12_sol1', 'graph_e_11_12 QUERY 1: no forbidden vertices');

-- GRAPH 6 - 11 - 10
PREPARE graph_e_11_12_q2 AS
SELECT type, id, contracted_vertices, source, target, cost
FROM pgr_contraction(
    'graph_e_11_12',
    ARRAY[2]::INTEGER[], 1, ARRAY[11]::INTEGER[], false);

SELECT is_empty('graph_e_11_12_q2', 'graph_e_1_4 QUERY 2: forbidden vertex: 11');

-- THREE EDGES

--------------------
-- GRAPH 4 - 3 - 6 - 11
-- no forbidden vertices
PREPARE graph_e_3_5_11_q1 AS
SELECT type, id, contracted_vertices, source, target, cost
FROM pgr_contraction(
    'graph_e_3_5_11',
    ARRAY[2]::INTEGER[], 1, ARRAY[]::INTEGER[], false);

PREPARE graph_e_3_5_11_sol1 AS
SELECT type, id, contracted_vertices, source, target, cost
FROM (VALUES
    ('e', -1, ARRAY[3, 6]::BIGINT[], 4, 11, 3))
AS t(type, id, contracted_vertices, source, target, cost);

SELECT set_eq('graph_e_3_5_11_q1', 'graph_e_3_5_11_sol1', 'graph_e_3_5_11 QUERY 1: Directed graph with three edges and no forbidden vertices');

-- GRAPH 4 - 3 - 6 -11
-- 3 is forbidden
PREPARE graph_e_3_5_11_q2 AS
SELECT type, id, contracted_vertices, source, target, cost
FROM pgr_contraction(
    'graph_e_3_5_11',
    ARRAY[2]::INTEGER[], 1, ARRAY[3]::INTEGER[], false);

PREPARE graph_e_3_5_11_sol2 AS
SELECT type, id, contracted_vertices, source, target, cost
FROM (VALUES
    ('e', -1, ARRAY[6]::BIGINT[], 3, 11, 2))
AS t(type, id, contracted_vertices, source, target, cost);

SELECT set_eq('graph_e_3_5_11_q2', 'graph_e_3_5_11_sol2', 'graph_e_3_5_11 QUERY 2: Directed graph with three edges and 3 is forbidden vertices');

-- GRAPH 4 - 3 - 6 - 11
-- 6 is forbidden
PREPARE graph_e_3_5_11_q3 AS
SELECT type, id, contracted_vertices, source, target, cost
FROM pgr_contraction(
    'graph_e_3_5_11',
    ARRAY[2]::INTEGER[], 1, ARRAY[6]::INTEGER[], false);

PREPARE graph_e_3_5_11_sol3 AS
SELECT type, id, contracted_vertices, source, target, cost
FROM (VALUES
    ('e', -1, ARRAY[3]::BIGINT[], 4, 6, 2))
AS t(type, id, contracted_vertices, source, target, cost);

SELECT set_eq('graph_e_3_5_11_q3', 'graph_e_3_5_11_sol3', 'graph_e_3_5_11 QUERY 3: Directed graph with three edges and 6 is forbidden vertices');

-- 3, 6 are forbidden
PREPARE graph_e_3_5_11_q4 AS
SELECT type, id, contracted_vertices, source, target, cost
FROM pgr_contraction(
    'graph_e_3_5_11',
    ARRAY[2]::INTEGER[], 1, ARRAY[3, 6]::INTEGER[], false);

SELECT is_empty('graph_e_3_5_11_q4', 'graph_3_5_11 QUERY 4: Directed graph with three edges and 3, 6 are forbidden vertices');


-- FOUR EDGES
-- GRAPH 11 - 12 - 9 - 6 - 11
-- no forbidden vertices
PREPARE graph_9_11_13_15_q1 AS
SELECT type, id, contracted_vertices, source, target, cost
FROM pgr_contraction(
    'graph_e_9_11_13_15',
    ARRAY[2]::INTEGER[], 1, ARRAY[]::INTEGER[], false);

PREPARE graph_9_11_13_15_sol1 AS
SELECT type, id, contracted_vertices, source, target, cost
FROM (VALUES
    ('e', -1, ARRAY[6, 9]::BIGINT[], 11, 12, 3))
AS t(type, id, contracted_vertices, source, target, cost);

SELECT set_eq('graph_9_11_13_15_q1', 'graph_9_11_13_15_sol1', 'graph_9_11_13_15 QUERY 1: no forbidden vertices');

-- GRAPH 6 - 11 - 12 - 9 - 6
-- 6 is forbidden
PREPARE graph_9_11_13_15_q2 AS
SELECT type, id, contracted_vertices, source, target, cost
FROM pgr_contraction(
    'graph_e_9_11_13_15',
    ARRAY[2]::INTEGER[], 1, ARRAY[6]::INTEGER[], false);

PREPARE graph_9_11_13_15_sol2 AS
SELECT type, id, contracted_vertices, source, target, cost
FROM (VALUES
    ('e', -1, ARRAY[9, 12]::BIGINT[], 6, 11, 3))
AS t(type, id, contracted_vertices, source, target, cost);

SELECT set_eq('graph_9_11_13_15_q2', 'graph_9_11_13_15_sol2', 'graph_9_11_13_15 QUERY 2: forbidden vertices: 6');


-- GRAPH 6 - 11 - 12 - 9 - 6
-- 12 is forbidden
PREPARE graph_9_11_13_15_q3 AS
SELECT type, id, contracted_vertices, source, target, cost
FROM pgr_contraction(
    'graph_e_9_11_13_15',
    ARRAY[2]::INTEGER[], 1, ARRAY[12]::INTEGER[], false);

PREPARE graph_9_11_13_15_sol3 AS
SELECT type, id, contracted_vertices, source, target, cost
FROM (VALUES
    ('e', -1, ARRAY[6, 9]::BIGINT[], 11, 12, 3))
AS t(type, id, contracted_vertices, source, target, cost);

SELECT set_eq('graph_9_11_13_15_q3', 'graph_9_11_13_15_sol3', 'graph_9_11_13_15 QUERY 3: forbidden vertices: 12');

-- GRAPH 6 - 11 - 12 - 9 - 6
-- 6, 12 is forbidden
PREPARE graph_9_11_13_15_q4 AS
SELECT type, id, contracted_vertices, source, target, cost
FROM pgr_contraction(
    'graph_e_9_11_13_15',
    ARRAY[2]::INTEGER[], 1, ARRAY[6, 12]::INTEGER[], false);

PREPARE graph_9_11_13_15_sol4 AS
SELECT type, id, contracted_vertices, source, target, cost
FROM (VALUES
    ('e', -1, ARRAY[9]::BIGINT[], 6, 12, 2),
    ('e', -2, ARRAY[11]::BIGINT[], 6, 12, 2))
AS t(type, id, contracted_vertices, source, target, cost);
SELECT set_eq('graph_9_11_13_15_q4', 'graph_9_11_13_15_sol4', 'graph_9_11_13_15 QUERY 4: forbidden vertices: 6, 12');


-- GRAPH 6 - 11 - 12 - 9 - 6
PREPARE graph_9_11_13_15_q5 AS
SELECT type, id, contracted_vertices, source, target, cost
FROM pgr_contraction(
    'graph_e_9_11_13_15',
    ARRAY[2]::INTEGER[], 1, ARRAY[9]::INTEGER[], false);

PREPARE graph_9_11_13_15_sol5 AS
SELECT type, id, contracted_vertices, source, target, cost
FROM (VALUES
    ('e', -1, ARRAY[6, 11]::BIGINT[], 9, 12, 3))
AS t(type, id, contracted_vertices, source, target, cost);

SELECT set_eq('graph_9_11_13_15_q5', 'graph_9_11_13_15_sol5', 'graph_9_11_13_15 QUERY 5: forbidden vertices: 9');

-- GRAPH 6 - 11 - 12 - 9 - 6
PREPARE graph_9_11_13_15_q6 AS
SELECT type, id, contracted_vertices, source, target, cost
FROM pgr_contraction(
    'graph_e_9_11_13_15',
    ARRAY[2]::INTEGER[], 1, ARRAY[6, 9]::INTEGER[], false);

PREPARE graph_9_11_13_15_sol6 AS
SELECT type, id, contracted_vertices, source, target, cost
FROM (VALUES
    ('e', -1, ARRAY[11, 12]::BIGINT[], 6, 9, 3))
AS t(type, id, contracted_vertices, source, target, cost);

SELECT set_eq('graph_9_11_13_15_q6', 'graph_9_11_13_15_sol6', 'graph_9_11_13_15 QUERY 6: forbidden vertices: 6 & 9');

-- GRAPH 6 - 11 - 12 - 9 - 6
PREPARE graph_9_11_13_15_q7 AS
SELECT type, id, contracted_vertices, source, target, cost
FROM pgr_contraction(
    'graph_e_9_11_13_15',
    ARRAY[2]::INTEGER[], 1, ARRAY[9, 12]::INTEGER[], false);

PREPARE graph_9_11_13_15_sol7 AS
SELECT type, id, contracted_vertices, source, target, cost
FROM (VALUES
    ('e', -1, ARRAY[6, 11]::BIGINT[], 9, 12, 3))
AS t(type, id, contracted_vertices, source, target, cost);
SELECT set_eq('graph_9_11_13_15_q7', 'graph_9_11_13_15_sol7', 'graph_9_11_13_15 QUERY 7: forbidden vertices: 12 & 9');

-- GRAPH 6 - 11 - 12 - 9 - 6
PREPARE graph_9_11_13_15_q8 AS
SELECT type, id, contracted_vertices, source, target, cost
FROM pgr_contraction(
    'graph_e_9_11_13_15',
    ARRAY[2]::INTEGER[], 1, ARRAY[6, 9, 12]::INTEGER[], false);

PREPARE graph_9_11_13_15_sol8 AS
SELECT type, id, contracted_vertices, source, target, cost
FROM (VALUES
    ('e', -1, ARRAY[11]::BIGINT[], 6, 12, 2))
AS t(type, id, contracted_vertices, source, target, cost);
SELECT set_eq('graph_9_11_13_15_q8', 'graph_9_11_13_15_sol8', 'graph_9_11_13_15 QUERY 8: forbidden vertices: 6, 12 & 9');

-- GRAPH 6 - 11 - 12 - 9 - 6
PREPARE graph_9_11_13_15_q9 AS
SELECT type, id, contracted_vertices, source, target, cost
FROM pgr_contraction(
    'graph_e_9_11_13_15',
    ARRAY[2]::INTEGER[], 1, ARRAY[11]::INTEGER[], false);

PREPARE graph_9_11_13_15_sol9 AS
SELECT type, id, contracted_vertices, source, target, cost
FROM (VALUES
    ('e', -1, ARRAY[6, 9]::BIGINT[], 11, 12, 3))
AS t(type, id, contracted_vertices, source, target, cost);

SELECT set_eq('graph_9_11_13_15_q9', 'graph_9_11_13_15_sol9', 'graph_9_11_13_15 QUERY 9: forbidden vertices: 11');

-- GRAPH 6 - 11 - 12 - 9 - 6
PREPARE graph_9_11_13_15_q10 AS
SELECT type, id, contracted_vertices, source, target, cost
FROM pgr_contraction(
    'graph_e_9_11_13_15',
    ARRAY[2]::INTEGER[], 1, ARRAY[6, 11]::INTEGER[], false);

PREPARE graph_9_11_13_15_sol10 AS
SELECT type, id, contracted_vertices, source, target, cost
FROM (VALUES
    ('e', -1, ARRAY[9, 12]::BIGINT[], 6, 11, 3))
AS t(type, id, contracted_vertices, source, target, cost);

SELECT set_eq('graph_9_11_13_15_q10', 'graph_9_11_13_15_sol10', 'graph_9_11_13_15 QUERY 10: forbidden vertices:  6 & 11');

-- GRAPH 6 - 11 - 12 - 9 - 6
PREPARE graph_9_11_13_15_q11 AS
SELECT type, id, contracted_vertices, source, target, cost
FROM pgr_contraction(
    'graph_e_9_11_13_15',
    ARRAY[2]::INTEGER[], 1, ARRAY[11, 12]::INTEGER[], false);
SELECT set_eq('graph_9_11_13_15_q11', 'graph_9_11_13_15_sol3', 'graph_9_11_13_15 QUERY 11: forbidden vertices:  12 & 11');

-- GRAPH 6 - 11 - 12 - 9 - 6
PREPARE graph_9_11_13_15_q12 AS
SELECT type, id, contracted_vertices, source, target, cost
FROM pgr_contraction(
    'graph_e_9_11_13_15',
    ARRAY[2]::INTEGER[], 1, ARRAY[6, 11, 12]::INTEGER[], false);

PREPARE graph_9_11_13_15_sol12 AS
SELECT type, id, contracted_vertices, source, target, cost
FROM (VALUES
    ('e', -1, ARRAY[9]::BIGINT[], 6, 12, 2))
AS t(type, id, contracted_vertices, source, target, cost);

SELECT set_eq('graph_9_11_13_15_q12', 'graph_9_11_13_15_sol12', 'graph_9_11_13_15 QUERY 12: forbidden vertices:  6, 12 & 11');


-- GRAPH 6 - 11 - 12 - 9 - 6
PREPARE graph_9_11_13_15_q13 AS
SELECT type, id, contracted_vertices, source, target, cost
FROM pgr_contraction(
    'graph_e_9_11_13_15',
    ARRAY[2]::INTEGER[], 1, ARRAY[9, 11]::INTEGER[], false);

PREPARE graph_9_11_13_15_sol13 AS
SELECT type, id, contracted_vertices, source, target, cost
FROM (VALUES
    ('e', -2, ARRAY[12]::BIGINT[], 9, 11, 2),
    ('e', -1, ARRAY[6]::BIGINT[], 9, 11, 2))
AS t(type, id, contracted_vertices, source, target, cost);

SELECT set_eq('graph_9_11_13_15_q13', 'graph_9_11_13_15_sol13', 'graph_9_11_13_15 QUERY 13: forbidden vertices:  9 & 11');



-- GRAPH 6 - 11 - 12 - 9 - 6
PREPARE graph_9_11_13_15_q14 AS
SELECT type, id, contracted_vertices, source, target, cost
FROM pgr_contraction(
    'graph_e_9_11_13_15',
    ARRAY[2]::INTEGER[], 1, ARRAY[6, 9, 11]::INTEGER[], false);

PREPARE graph_9_11_13_15_sol14 AS
SELECT type, id, contracted_vertices, source, target, cost
FROM (VALUES
    ('e', -1, ARRAY[12]::BIGINT[], 9, 11, 2))
AS t(type, id, contracted_vertices, source, target, cost);

SELECT set_eq('graph_9_11_13_15_q14', 'graph_9_11_13_15_sol14', 'graph_9_11_13_15 QUERY 13: forbidden vertices:  6 & 9 & 11');


-- GRAPH 6 - 11 - 12 - 9 - 6
PREPARE graph_9_11_13_15_q15 AS
SELECT type, id, contracted_vertices, source, target, cost
FROM pgr_contraction(
    'graph_e_9_11_13_15',
    ARRAY[2]::INTEGER[], 1, ARRAY[9, 11, 12]::INTEGER[], false);

PREPARE graph_9_11_13_15_sol15 AS
SELECT type, id, contracted_vertices, source, target, cost
FROM (VALUES
    ('e', -1, ARRAY[6]::BIGINT[], 9, 11, 2))
AS t(type, id, contracted_vertices, source, target, cost);

SELECT set_eq('graph_9_11_13_15_q15', 'graph_9_11_13_15_sol15', 'graph_9_11_13_15 QUERY 13: forbidden vertices:  9 & 11 & 12');


-- GRAPH 6 - 11 - 12 - 9 - 6
PREPARE graph_9_11_13_15_q16 AS
SELECT type, id, contracted_vertices, source, target, cost
FROM pgr_contraction(
    'graph_e_9_11_13_15',
    ARRAY[2]::INTEGER[], 1, ARRAY[6, 9, 11, 12]::INTEGER[], false);
SELECT is_empty('graph_9_11_13_15_q16', 'graph_9_11_13_15 QUERY 16: forbidden vertices: 6 & 12 & 9 & 11');

SELECT finish();
ROLLBACK;
