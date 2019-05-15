\i setup.sql

SELECT plan(3);

UPDATE edge_table SET cost = cost + 0.001 * id * id, reverse_cost = reverse_cost + 0.001 * id * id;

--
PREPARE prim AS
SELECT edge, cost::TEXT
FROM pgr_prim(
  'SELECT id, source, target, cost, reverse_cost FROM edge_table'
);

PREPARE kruskal AS
SELECT edge, cost::TEXT
FROM pgr_kruskal(
  'SELECT id, source, target, cost, reverse_cost FROM edge_table'
);

SELECT set_eq('prim', 'kruskal', '1: Prim & kruskal should return same values');

PREPARE kruskal1 AS
SELECT edge, cost
FROM pgr_kruskal(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table'
);

PREPARE prim1 AS
SELECT edge, cost
FROM pgr_prim(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table'
);

SELECT bag_has('kruskal1',
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
'2: kruskal result');


SELECT bag_has('prim1',
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
'3: prim result');


SELECT * FROM finish();
ROLLBACK;
