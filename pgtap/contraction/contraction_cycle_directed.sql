\i setup.sql

SELECT plan(30);

SET client_min_messages TO WARNING;

PREPARE qempty AS
SELECT * FROM ( VALUES ('v', -1, '{}', -1, -1, -1) ) AS t(type, id, contracted_vertices, source, target, cost) WHERE 1 != 1 ;

-- TESTING CONTRACTION CYCLE WITH ONLY DEAD END CONTRACTION

-- SINGLE EDGE

PREPARE v2e1q10 AS
SELECT * FROM pgr_contraction(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1',
    ARRAY[1]::integer[], 1, ARRAY[]::integer[], true);

PREPARE v2e1q11 AS
SELECT * FROM pgr_contraction(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1',
    ARRAY[1, 1]::integer[], 1, ARRAY[]::integer[], true);

SELECT set_eq('v2e1q10', 'v2e1q11', '1: Directed graph with single edge');

PREPARE v2e1q12 AS
SELECT * FROM pgr_contraction(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1',
    ARRAY[1]::integer[], 2, ARRAY[]::integer[], true);

SELECT set_eq('v2e1q10', 'v2e1q12', '1: Directed graph with single edge');

SELECT set_eq('v2e1q11', 'v2e1q12', '1: Directed graph with single edge');

-- TWO EDGES

PREPARE v3e2q10 AS
SELECT * FROM pgr_contraction(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 2',
    ARRAY[1]::integer[], 1, ARRAY[]::integer[], true);

PREPARE v3e2q11 AS
SELECT * FROM pgr_contraction(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 2',
    ARRAY[1, 1]::integer[], 1, ARRAY[]::integer[], true);

SELECT set_eq('v2e1q10', 'v2e1q11', '1: Directed graph with two edges');

PREPARE v3e2q12 AS
SELECT * FROM pgr_contraction(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 2',
    ARRAY[1]::integer[], 2, ARRAY[]::integer[], true);

SELECT set_eq('v3e2q10', 'v3e2q12', '1: Directed graph with two edges');

SELECT set_eq('v3e2q11', 'v3e2q12', '1: Directed graph with two edges');


-- THREE EDGES

PREPARE v4e3q10 AS
SELECT * FROM pgr_contraction(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 2 or id = 3',
    ARRAY[1]::integer[], 1, ARRAY[]::integer[], true);

PREPARE v4e3q11 AS
SELECT * FROM pgr_contraction(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 2 or id = 3',
    ARRAY[1, 1]::integer[], 1, ARRAY[]::integer[], true);

SELECT set_eq('v4e3q10', 'v4e3q11', '1: Directed graph with three edges');

PREPARE v4e3q12 AS
SELECT * FROM pgr_contraction(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 2 or id = 3',
    ARRAY[1]::integer[], 2, ARRAY[]::integer[], true);

SELECT set_eq('v4e3q10', 'v4e3q12', '1: Directed graph with three edges');
SELECT set_eq('v4e3q11', 'v4e3q12', '1: Directed graph with three edges');

PREPARE v4e3q13 AS
SELECT * FROM pgr_contraction(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 or id = 5 or id = 6',
    ARRAY[1]::integer[], 1, ARRAY[]::integer[], true);

PREPARE v4e3q14 AS
SELECT * FROM pgr_contraction(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 or id = 5 or id = 6',
    ARRAY[1, 1]::integer[], 1, ARRAY[]::integer[], true);

SELECT set_eq('v4e3q13', 'v4e3q14', '1: Directed graph with three edges');

PREPARE v4e3q15 AS
SELECT * FROM pgr_contraction(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 or id = 5 or id = 6',
    ARRAY[1]::integer[], 2, ARRAY[]::integer[], true);

SELECT set_eq('v4e3q13', 'v4e3q15', '1: Directed graph with three edges');
SELECT set_eq('v4e3q14', 'v4e3q15', '1: Directed graph with three edges');

-- FOUR EDGES

PREPARE v4e4q10 AS
SELECT * FROM pgr_contraction(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 or id = 5 or id = 6 or id = 8',
    ARRAY[1]::integer[], 1, ARRAY[]::integer[], true);

PREPARE v4e4q11 AS
SELECT * FROM pgr_contraction(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 or id = 5 or id = 6 or id = 8',
    ARRAY[1, 1]::integer[], 1, ARRAY[]::integer[], true);

SELECT set_eq('v4e4q10', 'v4e4q11', '9: Directed graph with four edges ');

PREPARE v4e4q12 AS
SELECT * FROM pgr_contraction(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 or id = 5 or id = 6 or id = 8',
    ARRAY[1]::integer[], 2, ARRAY[]::integer[], true);

SELECT set_eq('v4e4q10', 'v4e4q12', '9: Directed graph with four edges ');
SELECT set_eq('v4e4q11', 'v4e4q12', '9: Directed graph with four edges ');

PREPARE v6e4q10 AS
SELECT * FROM pgr_contraction(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 2 or id = 3 or id = 4',
    ARRAY[1]::integer[], 1, ARRAY[]::integer[], true);

PREPARE v6e4q11 AS
SELECT * FROM pgr_contraction(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 2 or id = 3 or id = 4',
    ARRAY[1, 1]::integer[], 1, ARRAY[]::integer[], true);

SELECT set_eq('v6e4q10', 'v6e4q11', '11: Directed graph with four edges and no forbidden vertices');

PREPARE v6e4q12 AS
SELECT * FROM pgr_contraction(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 2 or id = 3 or id = 4',
    ARRAY[1]::integer[], 2, ARRAY[]::integer[], true);

SELECT set_eq('v6e4q10', 'v6e4q12', '11: Directed graph with four edges and no forbidden vertices');
SELECT set_eq('v6e4q11', 'v6e4q12', '11: Directed graph with four edges and no forbidden vertices');

-- TESTING CONTRACTION CYCLE WITH ONLY LINEAR CONTRACTION

-- TWO EDGES
PREPARE v3e2q20 AS
SELECT * FROM pgr_contraction(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 2',
    ARRAY[2]::integer[], 1, ARRAY[]::integer[], true);

PREPARE v3e2q21 AS
SELECT * FROM pgr_contraction(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 2',
    ARRAY[2, 2]::integer[], 1, ARRAY[]::integer[], true);

SELECT set_eq('v3e2q20', 'v3e2q21', '1: Directed graph with two edges and no forbidden vertices');

PREPARE v3e2q22 AS
SELECT * FROM pgr_contraction(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 2',
    ARRAY[2]::integer[], 2, ARRAY[]::integer[], true);

SELECT set_eq('v3e2q20', 'v3e2q22', '1: Directed graph with two edges and no forbidden vertices');
SELECT set_eq('v3e2q21', 'v3e2q22', '1: Directed graph with two edges and no forbidden vertices');


-- THREE EDGES

PREPARE v4e3q20 AS
SELECT * FROM pgr_contraction(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 2 or id = 3',
    ARRAY[2]::integer[], 1, ARRAY[]::integer[], true);

PREPARE v4e3q21 AS
SELECT * FROM pgr_contraction(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 2 or id = 3',
    ARRAY[2, 2]::integer[], 1, ARRAY[]::integer[], true);

SELECT set_eq('v4e3q20', 'v4e3q21', '5: Directed graph with three edges and no forbidden vertices');

PREPARE v4e3q22 AS
SELECT * FROM pgr_contraction(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 2 or id = 3',
    ARRAY[2]::integer[], 2, ARRAY[]::integer[], true);

SELECT set_eq('v4e3q20', 'v4e3q22', '5: Directed graph with three edges and no forbidden vertices');
SELECT set_eq('v4e3q21', 'v4e3q22', '5: Directed graph with three edges and no forbidden vertices');

-- FOUR EDGES
PREPARE v4e4q20 AS
SELECT * FROM pgr_contraction(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 9 or id = 10 or id = 11',
    ARRAY[2]::integer[], 1, ARRAY[]::integer[], true);

PREPARE v4e4q21 AS
SELECT * FROM pgr_contraction(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 9 or id = 10 or id = 11',
    ARRAY[2, 2]::integer[], 1, ARRAY[]::integer[], true);

SELECT set_eq('v4e4q20', 'v4e4q21', '5: Directed graph with four edges and no forbidden vertices');

PREPARE v4e4q22 AS
SELECT * FROM pgr_contraction(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1 or id = 9 or id = 10 or id = 11',
    ARRAY[2]::integer[], 2, ARRAY[]::integer[], true);

SELECT set_eq('v4e4q20', 'v4e4q22', '5: Directed graph with four edges and no forbidden vertices');
SELECT set_eq('v4e4q21', 'v4e4q22', '5: Directed graph with four edges and no forbidden vertices');

PREPARE v4e4q23 AS
SELECT * FROM pgr_contraction(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 2 or id = 9 or id = 12 or id = 13',
    ARRAY[2]::integer[], 1, ARRAY[]::integer[], true);

PREPARE v4e4q24 AS
SELECT * FROM pgr_contraction(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 2 or id = 9 or id = 12 or id = 13',
    ARRAY[2, 2]::integer[], 1, ARRAY[]::integer[], true);

SELECT set_eq('v4e4q23', 'v4e4q24', '5: Directed graph with four edges and no forbidden vertices');

PREPARE v4e4q25 AS
SELECT * FROM pgr_contraction(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 2 or id = 9 or id = 12 or id = 13',
    ARRAY[2]::integer[], 2, ARRAY[]::integer[], true);

SELECT set_eq('v4e4q23', 'v4e4q25', '5: Directed graph with four edges and no forbidden vertices');
SELECT set_eq('v4e4q24', 'v4e4q25', '5: Directed graph with four edges and no forbidden vertices');
