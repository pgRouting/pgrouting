\echo --q1 Checking data read
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    ARRAY[1]::integer[], ARRAY[0]::integer[], 1, true);

\echo --q2 Checking minimum number of cycles
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    ARRAY[1]::integer[], ARRAY[0]::integer[], 0, true);