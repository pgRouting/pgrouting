SELECT * FROM edge_table;

\echo --q0 Checking dead end contraction for workshop data
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    ARRAY[]::BIGINT[], ARRAY[1]::integer[], 1, true);

