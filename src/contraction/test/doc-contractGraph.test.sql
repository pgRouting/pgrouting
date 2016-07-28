\echo -- q1
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    ARRAY[]::BIGINT[], ARRAY[1]::integer[], 1, true);

\echo -- q2
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    ARRAY[]::bigint[], ARRAY[2]::integer[], 1, true);

\echo -- q3
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    ARRAY[]::BIGINT[], ARRAY[1]::integer[], 1, false);

\echo -- q4
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    ARRAY[]::bigint[], ARRAY[2]::integer[], 1, false);

\echo -- q5
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost FROM edge_table',
    ARRAY[]::BIGINT[], ARRAY[1]::integer[], 1, true);

\echo -- q6
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost FROM edge_table',
    ARRAY[]::bigint[], ARRAY[2]::integer[], 1, true);

\echo -- q7
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost FROM edge_table',
    ARRAY[]::BIGINT[], ARRAY[1]::integer[], 1, false);

\echo -- q8
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost FROM edge_table',
    ARRAY[]::bigint[], ARRAY[2]::integer[], 1, false);

\echo -- q9
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    ARRAY[]::bigint[], ARRAY[1, 2]::integer[], 1, false) WHERE type = 'e';

\echo -- q10
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    ARRAY[]::bigint[], ARRAY[1, 2]::integer[], 1, false) WHERE type = 'v';


\echo -- q11
