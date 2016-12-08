
/*
bigint (-9,223,372,036,854,775,808) to (9,223,372,036,854,775,807)
int	(-2,147,483,648) to (2,147,483,647)
*/

/*
Testing for ids > 10^8
*/

\echo --q1
UPDATE edge_table
SET id = id + POWER(10, 8),
	source = source + POWER(10, 8),
	target = target + POWER(10, 8);

\echo --q2
UPDATE edge_table_vertices_pgr
SET id = id + POWER(10, 8);

\echo --q3
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    ARRAY[1]::integer[], 1, ARRAY[]::BIGINT[], true);

\echo --q4
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    ARRAY[2]::integer[], 1, ARRAY[]::BIGINT[], true);

\echo --q5
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    ARRAY[1, 2]::integer[], 1, ARRAY[]::BIGINT[], true);

ROLLBACK;

/*
Testing for ids > 10^12
*/
BEGIN;

\echo --q6
UPDATE edge_table
SET id = id + POWER(10, 12),
	source = source + POWER(10, 12),
	target = target + POWER(10, 12);

\echo --q7
UPDATE edge_table_vertices_pgr
SET id = id + POWER(10, 12);

\echo --q8
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    ARRAY[1]::integer[], 1, ARRAY[]::BIGINT[], true);

\echo --q9
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    ARRAY[2]::integer[], 1, ARRAY[]::BIGINT[], true);

\echo --q10
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    ARRAY[1, 2]::integer[], 1, ARRAY[]::BIGINT[], true);

\echo --q11

