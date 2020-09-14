\i setup.sql

SELECT plan(20);

-- 0 edge, 0 vertex tests

PREPARE q1 AS
SELECT id, source, target, cost, reverse_cost
FROM edge_table
WHERE id > 18;

-- Graph is empty - it has 0 edge and 0 vertex
SELECT is_empty('q1', 'q1: Graph with 0 edge and 0 vertex');

-- 0 edge, 0 vertex tests

PREPARE makeConnected2 AS
SELECT *
FROM pgr_makeConnected('q1');

SELECT is_empty('makeConnected2', '2: Graph with 0 edge and 0 vertex -> Empty row is returned');

-- vertex not present in graph tests

PREPARE q3 AS
SELECT id, source, target, cost, reverse_cost
FROM edge_table WHERE source = 50;

SELECT is_empty('q3','3: Vertex 50 does not exist in sample data');

PREPARE vertexNotPresent4 AS
SELECT *
FROM pgr_makeConnected('q3');

SELECT is_empty('vertexNotPresent4', '4: Vertex Not present in the graph -> Empty row is returned');

-- 1 vertex tests

PREPARE q5 AS
SELECT id, source, 6 AS target, cost, reverse_cost
FROM edge_table
WHERE id = 9;
-- Graph with only vertex 9
SELECT set_eq('q5', $$VALUES (9, 6, 6, 1, 1)$$, 'q5: Graph with only vertex 6');

PREPARE oneVertexTest6 AS
SELECT *
FROM pgr_makeConnected('q5');

SELECT is_empty('oneVertexTest6', '6: Graph is already Connected -> Empty row is returned');

-- 2 vertices tests ===> Already Connected

PREPARE q7 AS
SELECT id, source, target, cost, reverse_cost
FROM edge_table
WHERE id = 1;

SELECT set_eq('q7', $$VALUES (1, 1, 2, 1, 1)$$, 'q9: Graph with two vertices 1 and 2');

PREPARE twoVerticesTest8 AS
SELECT *
FROM pgr_makeConnected('q7');

SELECT is_empty('twoVerticesTest8', '8: Graph is already Connected -> Empty row is returned');

-- 2 vertices tests ===> Not Connected

PREPARE q9 AS
SELECT id, source, 2 AS target, cost, reverse_cost FROM edge_table WHERE id = 2
                          UNION
SELECT id, source, 6 AS target, cost, reverse_cost FROM edge_table WHERE id = 9;

SELECT set_eq('q9', $$VALUES (2, 2, 2, -1, 1), (9, 6, 6, 1, 1)$$, 'q9: Graph with two vertices 2 and 6');

PREPARE twoVerticesTest10 AS
SELECT *
FROM pgr_makeConnected('q9');

SELECT set_eq('twoVerticesTest10', $$VALUES (1, 2, 6)$$, '10: One row is returned');


-- 3 vertices tests ====> Already Connnected

PREPARE q11 AS
SELECT id, source, target, cost, reverse_cost
FROM edge_table
WHERE id IN (1,2);

SELECT set_eq('q11', $$VALUES (1, 1, 2, 1, 1), (2, 2, 3, -1, 1)$$, 'q11: Graph with three vertices 1, 2 and 3');

PREPARE threeVerticesTest12 AS
SELECT *
FROM pgr_makeConnected('q11');

SELECT is_empty('threeVerticesTest12', '12: Graph is already Connected -> Empty row is returned');



-- 3 vertices tests ====> Not Connnected

PREPARE q13 AS
SELECT id,  source, target, cost, reverse_cost FROM edge_table WHERE id = 2
                                UNION
SELECT id, source, 6 AS target, cost, reverse_cost FROM edge_table WHERE id = 9;

SELECT set_eq('q13', $$VALUES (2, 2, 3, -1, 1), (9, 6, 6, 1, 1)$$, 'q13: Graph with two vertices 2 and 6');

PREPARE threeVerticesTest14 AS
SELECT *
FROM pgr_makeConnected('q13');

SELECT set_eq('threeVerticesTest14', $$VALUES (1, 3, 6)$$, '14: One row is returned');


-- 4 vertex tests ===> Cyclic

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
FROM pgr_makeConnected('q15');
SELECT is_empty('fourVerticesCyclicTest16', '14: Graph is already Connected -> Empty row is returned');

-- 4 vertex tests ===> Not Connected

PREPARE q17 AS
SELECT id, source, target, cost, reverse_cost
FROM edge_table
WHERE id IN (1,6);

SELECT set_eq('q17', $$VALUES (1, 1, 2, 1, 1), (6, 7, 8, 1, 1)$$, 'q17: Graph with four vertices 1, 2, 7 and 8');

PREPARE fourVerticesTest18 AS
SELECT *
FROM pgr_makeConnected('q17');

SELECT set_eq('fourVerticesTest18', $$VALUES (1, 2, 7) $$, '18:Two Connected Components. One row is returned');


PREPARE q19 AS
SELECT id, source, target, cost, reverse_cost
FROM edge_table
WHERE id IN (17,18);

SELECT set_eq('q19', $$VALUES (17, 14, 15, 1, 1), (18, 16, 17, 1, 1)$$, 'q19: Graph with four vertices 14, 15, 16 and 17');

PREPARE fourVerticesTest20 AS
SELECT *
FROM pgr_makeConnected('q19');

SELECT set_eq('fourVerticesTest20', $$VALUES (1, 15, 16) $$,'20:Two Connected Components. One row is returned');



SELECT * FROM finish();
ROLLBACK;
