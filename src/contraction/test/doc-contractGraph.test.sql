\echo -- q1
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    ARRAY[]::BIGINT[], ARRAY[0]::integer[], 1, true);

\echo -- q2
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    ARRAY[]::bigint[], ARRAY[1]::integer[], 1, true);

\echo -- q3