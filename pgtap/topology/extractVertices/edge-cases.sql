\i setup.sql

SELECT plan(4);

UPDATE edge_table SET cost = cost + 0.001 * id * id, reverse_cost = reverse_cost + 0.001 * id * id;

--
PREPARE query_1 AS
SELECT * FROM pgr_extractVertices(
    'SELECT source, target, the_geom
    FROM edge_table WHERE id > 18'
);

PREPARE query_2 AS
SELECT *
FROM pgr_extractVertices(
    'SELECT source, target
     FROM edge_table WHERE id > 18'
);

SELECT throws_ok('query_1', 'P0001', 'column "id" does not exist', 'No_edge -> No answer');
SELECT throws_ok('query_2', 'P0001', 'column "id" does not exist', 'No_edge -> No answer');


--
PREPARE query_3 AS
SELECT count(*)
FROM pgr_extractVertices(
    'SELECT source, target, the_geom
    FROM edge_table'
);

SELECT set_eq('query_3',
    $$VALUES (17)$$,
    '3: Number of vertices extracted');

--
PREPARE query_4 AS
SELECT id
FROM pgr_extractVertices(
    'SELECT source, target, the_geom
    FROM edge_table'
);

SELECT bag_has('query_4',
$$VALUES
(1),
(2),
(3),
(4),
(5),
(6),
(7),
(8),
(9),
(10),
(11),
(12),
(14),
(15),
(16),
(17)$$,
'4: query result');


SELECT * FROM finish();
ROLLBACK;
