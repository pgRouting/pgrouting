\i setup.sql
SELECT plan(2);

PREPARE q1 AS
SELECT id, source, target, cost, reverse_cost
FROM edge_table
WHERE id > 18;

SELECT is_empty('q1', 'q1: Graph with 0 edge and 0 vertex');

PREPARE q2 AS
SELECT *
FROM pgr_ltdtree(
'SELECT id, source, target, cost, reverse_cost
FROM edge_table
WHERE id > 0',1
);
SELECT isnt_empty('q2', 'q1: Graph with 0 edge and 0 vertex');
ROLLBACK;
