\i setup.sql

SELECT plan(10);

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


-- 2 vertices test (connected)

PREPARE q5 AS
SELECT id, source, target, cost, reverse_cost
FROM edge_table
WHERE id = 7;

SELECT set_eq('q5', $$VALUES (7, 8, 5, 1, 1)$$, 'q5: Graph with two connected vertices 8 and 5');

PREPARE sequentialVertexColoring6 AS
SELECT *
FROM pgr_sequentialVertexColoring(
    'q5'
);

SELECT set_eq('sequentialVertexColoring6', $$VALUES (5, 1), (8, 0)$$, '6: Both vertices have different color');


-- 2 vertices test (isolated)

CREATE TABLE two_isolated_vertices_table (
    id BIGSERIAL,
    source BIGINT,
    target BIGINT,
    cost FLOAT,
    reverse_cost FLOAT
);

INSERT INTO two_isolated_vertices_table (source, target, cost, reverse_cost) VALUES
    (2, 2, -1, 1),
    (1, 1, 1, -1);

PREPARE q7 AS
SELECT id, source, target, cost, reverse_cost
FROM two_isolated_vertices_table;

SELECT set_eq('q7', $$VALUES (1, 2, 2, -1, 1), (2, 1, 1, 1, -1)$$, 'q7: Graph with two isolated vertices 1 and 2');

PREPARE sequentialVertexColoring8 AS
SELECT *
FROM pgr_sequentialVertexColoring(
    'q7'
);

SELECT set_eq('sequentialVertexColoring8', $$VALUES (1, 0), (2, 0)$$, '8: Both vertices have same color');


-- 3 vertices test

PREPARE q9 AS
SELECT id, source, target, cost, reverse_cost
FROM edge_table
WHERE id <= 2;

SELECT set_eq('q9', $$VALUES (1, 1, 2, 1, 1), (2, 2, 3, -1, 1)$$, 'q9: Graph with three vertices 1, 2 and 3');

PREPARE sequentialVertexColoring10 AS
SELECT *
FROM pgr_sequentialVertexColoring(
    'q9'
);

SELECT set_eq('sequentialVertexColoring10', $$VALUES (1, 0), (2, 1), (3, 0)$$, '10: 2 colors are required');


SELECT * FROM finish();
ROLLBACK;
