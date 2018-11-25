\i setup.sql

SELECT plan(5);

UPDATE edge_table SET cost = cost + 0.001 * id * id, reverse_cost = reverse_cost + 0.001 * id * id;

--
PREPARE prim1 AS
SELECT * FROM pgr_prim(
  'SELECT id, source, target, cost, reverse_cost
   FROM edge_table WHERE id > 18'
);

PREPARE prim2 AS
SELECT *
FROM pgr_prim(
    'SELECT id, source, target, cost
     FROM edge_table WHERE cost < 0 ORDER BY id'
);

SELECT is_empty('prim1', 'No_edge -> No answer');
SELECT is_empty('prim2', 'ALL Negative Cost -> no answer');


--
PREPARE prim4 AS
SELECT count(*)
FROM pgr_prim(
    'SELECT id, source, target, cost, reverse_cost
     FROM edge_table'
) WHERE edge < 0;

SELECT lives_ok('prim4',
    '3: Should live');

SELECT set_eq('prim4',
    $$VALUES (0)$$,
    '4: No edge with negative values');

--
PREPARE prim5 AS
SELECT edge, cost
FROM pgr_prim(
    'SELECT id, source, target, cost, reverse_cost
     FROM edge_table'
);

SELECT bag_has('prim5',
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
'4: prim result');


SELECT * FROM finish();
ROLLBACK;
