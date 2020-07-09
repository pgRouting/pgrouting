\i setup.sql

SELECT plan(6);

-- 0 edge, 0 vertex test

PREPARE q1 AS
SELECT id, source, target, cost, reverse_cost
FROM edge_table
WHERE id > 18;

SELECT is_empty('q1', 'q1: Graph with 0 edge and 0 vertex');

PREPARE sequentialVertexColoring2 AS
SELECT *
FROM pgr_sequentialVertexColoring(
    'q1'
);

SELECT is_empty('sequentialVertexColoring2', '2: Graph with 0 edge and 0 vertex -> Empty row is returned');


-- 1 vertex test

PREPARE q3 AS
SELECT id, source, 2 AS target, cost, reverse_cost
FROM edge_table
WHERE id = 2;

SELECT set_eq('q3', $$VALUES (2, 2, 2, -1, 1)$$, 'q3: Graph with only vertex 2');

PREPARE sequentialVertexColoring4 AS
SELECT *
FROM pgr_sequentialVertexColoring(
    'q3'
);

SELECT set_eq('sequentialVertexColoring4', $$VALUES (2, 0)$$, '4: Node 2 with color 0 is returned');


-- 2 vertices test

PREPARE q5 AS
SELECT id, source, target, cost, reverse_cost
FROM edge_table
WHERE id = 5;

SELECT set_eq('q5', $$VALUES (5, 3, 6, 1, -1)$$, 'q5: Graph with two vertices 3 and 6 and edge from 3 to 6');

PREPARE sequentialVertexColoring6 AS
SELECT *
FROM pgr_sequentialVertexColoring(
    'q5'
);

SELECT set_eq('sequentialVertexColoring6', $$VALUES (3, 0), (6, 1)$$, '6: Both vertices have different color');


SELECT * FROM finish();
ROLLBACK;
