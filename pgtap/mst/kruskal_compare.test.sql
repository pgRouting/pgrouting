\i setup.sql

SELECT plan(8);

PREPARE kruskal1 AS
SELECT * FROM pgr_kruskal(
  'SELECT id, source, target, cost, reverse_cost
    FROM edge_table WHERE id > 18'
);

PREPARE kruskal2 AS
SELECT *
FROM pgr_kruskal(
    'SELECT id, source, target, cost, reverse_cost
       FROM edge_table ORDER BY id'
) WHERE cost < 0;

SELECT is_empty('kruskal1', '1: No_edge');
SELECT is_empty('kruskal2', 'No cost can be negative');

PREPARE kruskal3 AS
SELECT *
FROM pgr_kruskal(
    'SELECT id, source, target, cost, reverse_cost
       FROM edge_table where id > 16'
);

SELECT set_eq('kruskal3', 'VALUES (1, 14, 17, 1, 1), (2, 16, 18, 1, 1)', '3: Compare when node is 16 with actual result');

SELECT * FROM finish();
ROLLBACK;
