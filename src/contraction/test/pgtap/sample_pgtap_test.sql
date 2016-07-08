\i setup.sql

SELECT plan(1);

SET client_min_messages TO WARNING;

PREPARE v2e1q10 AS
SELECT seq, id, source, target, cost, contracted_vertices_size FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1',
    ARRAY[]::integer[], ARRAY[0]::integer[], 1, true);

PREPARE v2e1q11 AS
SELECT seq, id, source, target, cost, contracted_vertices_size FROM ( VALUES (1, 1, -1, -1, -1, 1) ) AS t(seq, id, source, target, cost, contracted_vertices_size) ;

SELECT set_eq('v2e1q10', 'v2e1q11', '1: Directed graph with single edge and no forbidden vertices');

-- fails when I add contracted_vertices
PREPARE v2e1q20 AS
SELECT seq, id, source, target, cost, contracted_vertices, contracted_vertices_size FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1',
    ARRAY[]::integer[], ARRAY[0]::integer[], 1, true);

PREPARE v2e1q21 AS
SELECT seq, id, source, target, cost, contracted_vertices, contracted_vertices_size FROM ( VALUES (1, 1::bigint, -1::bigint, -1::bigint, -1::double precision, ARRAY[2]::bigint[], 1::integer) ) AS t(seq, id, source, target, cost, contracted_vertices, contracted_vertices_size) ;

SELECT set_eq('v2e1q20', 'v2e1q21', '2: Directed graph with single edge and no forbidden vertices');

SELECT finish();
ROLLBACK;