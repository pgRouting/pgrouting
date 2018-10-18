\i setup.sql

SELECT plan(8);

-- without root
PREPARE kruskal1 AS
SELECT * FROM pgr_kruskal(
  'SELECT id, source, target, cost, reverse_cost
   FROM edge_table WHERE id > 18'
);

PREPARE kruskal2 AS
SELECT *
FROM pgr_kruskal(
    'SELECT id, source, target, cost
     FROM edge_table ORDER BY id'
) WHERE cost < 0;

SELECT is_empty('kruskal1', 'No_edge -> No answer');
SELECT is_empty('kruskal2', 'Negative Cost -> no answer');

PREPARE kruskal3 AS
SELECT DISTINCT depth, agg_cost
FROM pgr_kruskal(
    'SELECT id, source, target, cost, reverse_cost
     FROM edge_table'
);

SELECT set_eq('kruskal3', $$
    VALUES (0, 0)
    $$, '3: No ordering -> no depth, no agg_cost');

PREPARE kruskal4 AS
SELECT count(*)
FROM pgr_kruskal(
    'SELECT id, source, target, cost, reverse_cost
     FROM edge_table'
) WHERE edge < 0;

SELECT set_eq('kruskal4',
    $$VALUES (0)$$,
    '4: No ordering -> no edge with negative values');


-- with root
PREPARE kruskal11 AS
SELECT * FROM pgr_kruskal(
  'SELECT id, source, target, cost, reverse_cost
   FROM edge_table WHERE id > 18', 5
);

PREPARE kruskal12 AS
SELECT *
FROM pgr_kruskal(
    'SELECT id, source, target, cost
     FROM edge_table ORDER BY id', 5
) WHERE cost < 0;

SELECT is_empty('kruskal11', 'No_edge -> No answer');
SELECT is_empty('kruskal12', 'Negative Cost -> no answer');

PREPARE kruskal13 AS
SELECT DISTINCT depth, agg_cost
FROM pgr_kruskal(
    'SELECT id, source, target, cost, reverse_cost
     FROM edge_table', 5
);

SELECT set_hasnt('kruskal13', $$
    VALUES (0, 0)
    $$, '7: No ordering -> has depth, has agg_cost');

PREPARE kruskal14 AS
SELECT count(*)
FROM pgr_kruskal(
    'SELECT id, source, target, cost, reverse_cost
     FROM edge_table', 5
) WHERE edge < 0;

SELECT set_eq('kruskal14',
    $$VALUES (1)$$,
    '8: No ordering -> has only 1 edge with negative value');


SELECT * FROM finish();
ROLLBACK;
