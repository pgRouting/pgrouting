\i setup.sql

SELECT plan(4);

UPDATE edge_table SET cost = cost + 0.001 * id * id, reverse_cost = reverse_cost + 0.001 * id * id;

--
PREPARE kruskal1 AS
SELECT * FROM pgr_kruskal(
  'SELECT id, source, target, cost, reverse_cost
   FROM edge_table WHERE id > 18'
);

PREPARE kruskal2 AS
SELECT *
FROM pgr_kruskal(
    'SELECT id, source, target, cost
     FROM edge_table WHERE cost < 0 ORDER BY id'
);

SELECT is_empty('kruskal1', 'No_edge -> No answer');
SELECT is_empty('kruskal2', 'ALL Negative Cost -> no answer');


--
PREPARE kruskal4 AS
SELECT count(*)
FROM pgr_kruskal(
    'SELECT id, source, target, cost, reverse_cost
     FROM edge_table'
) WHERE edge < 0;

SELECT set_eq('kruskal4',
    $$VALUES (0)$$,
    '3: No edge with negative values');

--
PREPARE kruskal5 AS
SELECT edge, cost
FROM pgr_kruskal(
    'SELECT id, source, target, cost, reverse_cost
     FROM edge_table'
);

SELECT bag_has('kruskal5',
$$VALUES
(1 , 1.001),
(2 , 1.004),
(3 , 1.009),
(4 , 1.016),
(5 , 1.025),
(6 , 1.036),
(7 , 1.049),
(9 , 1.081),
(10 ,   1.1),
(11 , 1.121),
(13 , 1.169),
(14 , 1.196),
(17 , 1.289),
(18 , 1.324)$$,
'4: kruskal result');


SELECT * FROM finish();
ROLLBACK;
