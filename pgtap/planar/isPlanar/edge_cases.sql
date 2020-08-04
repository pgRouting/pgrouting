\i setup.sql

SELECT plan(16);



-- 0 edge, 0 vertex tests

PREPARE q1 AS
SELECT id, source, target, cost, reverse_cost
FROM edge_table
WHERE id > 18;

-- Graph is empty - it has 0 edge and 0 vertex
SELECT is_empty('q1', 'q1: Graph with 0 edge and 0 vertex');

-- 0 edge, 0 vertex tests

PREPARE isPlanar1 AS
SELECT *
FROM pgr_isPlanar('q1');

SELECT set_eq('isPlanar1',$$VALUES('f'::bool) $$, '2: False, since vertex does not exist');


-- vertex not present in graph test

PREPARE q3 AS
SELECT id, source, target, cost, reverse_cost
FROM edge_table WHERE source = 50;

SELECT is_empty('q3','3: Vertex 50 does not exist in sample data');

PREPARE vertexNotPresent4 AS
SELECT *
FROM pgr_isPlanar('q3');

SELECT set_eq('vertexNotPresent4',$$VALUES('f'::bool) $$, '4:False, Vertex not present in graph');

-- 1 vertex test

PREPARE q5 AS
SELECT id, source, 6 AS target, cost, reverse_cost
FROM edge_table
WHERE id = 9;

-- Graph with only vertex 9
SELECT set_eq('q5', $$VALUES (9, 6, 6, 1, 1)$$, 'q5: Graph with only vertex 6');

PREPARE oneVertexTest6 AS
SELECT *
FROM pgr_isPlanar('q5');

SELECT set_eq('oneVertexTest6',$$VALUES('t'::bool) $$, '6:Planar Graph with only vertex 6');

PREPARE q7 AS
SELECT id, source, 3 AS target, cost, reverse_cost
FROM edge_table
WHERE id = 3;

-- Graph with only vertex 3
SELECT set_eq('q7', $$VALUES (3, 3, 3, -1, 1)$$, 'q7: Graph with only vertex 3');

PREPARE oneVertexTest8 AS
SELECT *
FROM pgr_isPlanar('q7');

SELECT set_eq('oneVertexTest8',$$VALUES('t'::bool) $$, '8:Planar Graph with only vertex 3');




-- 2 vertices tests

PREPARE q9 AS
SELECT id, source, target, cost, reverse_cost
FROM edge_table
WHERE id = 1;

SELECT set_eq('q9', $$VALUES (1, 1, 2, 1, 1)$$, 'q9: Graph with two vertices 1 and 2');

PREPARE twoVerticesTest10 AS
SELECT *
FROM pgr_isPlanar('q9');

SELECT set_eq('twoVerticesTest10', $$VALUES('t'::bool) $$, '10:Planar Graph with two vertices 1 and 2');


-- 3 vertices test

PREPARE q11 AS
SELECT id, source, target, cost, reverse_cost
FROM edge_table
WHERE id IN (1,2);

SELECT set_eq('q11', $$VALUES (1, 1, 2, 1, 1), (2, 2, 3, -1, 1)$$, 'q11: Graph with three vertices 1, 2 and 3');

PREPARE threeVerticesTest12 AS
SELECT *
FROM pgr_isPlanar(
    'q11'
);

SELECT set_eq('threeVerticesTest12', $$VALUES('t'::bool) $$, '12: Planar graph with 3 vertices');

-- 4 vertices test

PREPARE q13 AS
SELECT id, source, target, cost, reverse_cost
FROM edge_table
WHERE id IN (1,2,3);

SELECT set_eq('q13',
  $$VALUES (1, 1, 2, 1, 1),
           (2, 2, 3, -1, 1),
           (3, 3, 4, -1, 1)
  $$,
 'q13: Graph with three vertices 1, 2 and 3');

PREPARE fourVerticesTest14 AS
SELECT *
FROM pgr_isPlanar(
    'q13'
);

SELECT set_eq('fourVerticesTest14', $$VALUES('t'::bool) $$, '14: Planar graph with 4 vertices');

-- 4 vertices test (cyclic)

PREPARE q15 AS
SELECT id, source, target, cost, reverse_cost
FROM edge_table
WHERE id IN (8, 10, 11, 12);

SELECT set_eq('q15',
    $$VALUES
        (8, 5, 6, 1, 1),
        (10, 5, 10, 1, 1),
        (11, 6, 11, 1, -1),
        (12, 10, 11, 1, -1)
    $$,
    'q15: Graph with four vertices 5, 6, 10 and 11 (cyclic)'
);

PREPARE fourVerticesCyclicTest16 AS
SELECT *
FROM pgr_isPlanar(
    'q15'
);

SELECT set_eq('fourVerticesCyclicTest16', $$VALUES('t'::bool) $$, '16: Planar cyclic graph with 4 vertices');


SELECT * FROM finish();
ROLLBACK;
