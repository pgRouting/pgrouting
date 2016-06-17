\echo --q0 Checking dead end contraction for workshop data
SELECT * FROM pgr_contractGraph(
    'SELECT gid as id, source, target, cost, reverse_cost FROM ways',
    ARRAY[]::BIGINT[], ARRAY[0]::integer[], 1, true);
-- \echo --q3 -------------------------------------------

\echo --q1 Checking linear contraction for workshop data
SELECT * FROM pgr_contractGraph(
    'SELECT gid as id, source, target, cost, reverse_cost FROM ways',
    ARRAY[]::BIGINT[], ARRAY[1]::integer[], 1, true);
-- \echo --q1 -------------------------------------------

\echo --q2 Checking dead end, linear contraction for workshop data
SELECT * FROM pgr_contractGraph(
    'SELECT gid as id, source, target, cost, reverse_cost FROM ways',
    ARRAY[]::BIGINT[], ARRAY[0, 1]::integer[], 1, true);
-- \echo --q2 -------------------------------------------

\echo --q3 Checking linear contraction, dead end for workshop data
SELECT * FROM pgr_contractGraph(
    'SELECT gid as id, source, target, cost, reverse_cost FROM ways',
    ARRAY[]::BIGINT[], ARRAY[1, 0]::integer[], 1, true);
-- \echo --q3 -------------------------------------------