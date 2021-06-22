\i setup.sql

UPDATE edge_table SET cost = sign(cost), reverse_cost = sign(reverse_cost);
SELECT CASE WHEN NOT min_version('3.2.0') THEN plan(1) ELSE plan(18) END;

CREATE OR REPLACE FUNCTION edge_cases()
RETURNS SETOF TEXT AS
$BODY$
BEGIN

IF NOT min_version('3.2.0') THEN
  RETURN QUERY
  SELECT skip(1, 'Function is new on 3.2.0');
  RETURN;
END IF;

-- 0 edge, 0 vertex test

PREPARE q1 AS
SELECT id, source, target, cost, reverse_cost
FROM edge_table
WHERE id > 18;

RETURN QUERY
SELECT is_empty('q1', 'q1: Graph with 0 edge and 0 vertex');

PREPARE sequentialVertexColoring2 AS
SELECT *
FROM pgr_sequentialVertexColoring(
    'q1'
);

RETURN QUERY
SELECT is_empty('sequentialVertexColoring2', '2: Graph with 0 edge and 0 vertex -> Empty row is returned');


-- 1 vertex test

PREPARE q3 AS
SELECT id, source, 2 AS target, cost, reverse_cost
FROM edge_table
WHERE id = 2;

RETURN QUERY
SELECT set_eq('q3', $$VALUES (2, 2, 2, -1, 1)$$, 'q3: Graph with only vertex 2');

PREPARE sequentialVertexColoring4 AS
SELECT *
FROM pgr_sequentialVertexColoring(
    'q3'
);

RETURN QUERY
SELECT set_eq('sequentialVertexColoring4', $$VALUES (2, 1)$$, '4: Node 2 with color 1 is returned');


-- 2 vertices test (connected)

PREPARE q5 AS
SELECT id, source, target, cost, reverse_cost
FROM edge_table
WHERE id = 7;

RETURN QUERY
SELECT set_eq('q5', $$VALUES (7, 8, 5, 1, 1)$$, 'q5: Graph with two connected vertices 8 and 5');

PREPARE sequentialVertexColoring6 AS
SELECT *
FROM pgr_sequentialVertexColoring(
    'q5'
);

RETURN QUERY
SELECT set_eq('sequentialVertexColoring6', $$VALUES (5, 2), (8, 1)$$, '6: Both vertices have different color');


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

RETURN QUERY
SELECT set_eq('q7', $$VALUES (1, 2, 2, -1, 1), (2, 1, 1, 1, -1)$$, 'q7: Graph with two isolated vertices 1 and 2');

PREPARE sequentialVertexColoring8 AS
SELECT *
FROM pgr_sequentialVertexColoring(
    'q7'
);

RETURN QUERY
SELECT set_eq('sequentialVertexColoring8', $$VALUES (1, 1), (2, 1)$$, '8: Both vertices have same color');


-- 3 vertices test (linear)

PREPARE q9 AS
SELECT id, source, target, cost, reverse_cost
FROM edge_table
WHERE id <= 2;

RETURN QUERY
SELECT set_eq('q9', $$VALUES (1, 1, 2, 1, 1), (2, 2, 3, -1, 1)$$, 'q9: Graph with three vertices 1, 2 and 3');

PREPARE sequentialVertexColoring10 AS
SELECT *
FROM pgr_sequentialVertexColoring(
    'q9'
);

RETURN QUERY
SELECT set_eq('sequentialVertexColoring10', $$VALUES (1, 1), (2, 2), (3, 1)$$, '10: Two colors are required');


-- 3 vertices test (cyclic)

CREATE TABLE three_vertices_table (
    id BIGSERIAL,
    source BIGINT,
    target BIGINT,
    cost FLOAT,
    reverse_cost FLOAT
);

INSERT INTO three_vertices_table (source, target, cost, reverse_cost) VALUES
    (3, 6, 20, 15),
    (3, 8, 10, -10),
    (6, 8, -1, 12);

PREPARE q11 AS
SELECT id, source, target, cost, reverse_cost
FROM three_vertices_table;

RETURN QUERY
SELECT set_eq('q11',
    $$VALUES
        (1, 3, 6, 20, 15),
        (2, 3, 8, 10, -10),
        (3, 6, 8, -1, 12)
    $$,
    'q11: Cyclic Graph with three vertices 3, 6 and 8'
);

PREPARE sequentialVertexColoring12 AS
SELECT *
FROM pgr_sequentialVertexColoring(
    'q11'
);

RETURN QUERY
SELECT set_eq('sequentialVertexColoring12', $$VALUES (3, 1), (6, 2), (8, 3)$$, '12: Three colors are required');


-- 4 vertices test (linear)

PREPARE q13 AS
SELECT id, source, target, cost, reverse_cost
FROM edge_table
WHERE id <= 3;

RETURN QUERY
SELECT set_eq('q13',
    $$VALUES
        (1, 1, 2, 1, 1),
        (2, 2, 3, -1, 1),
        (3, 3, 4, -1, 1)
    $$,
    'q13: Graph with four vertices 1, 2, 3 and 4'
);

PREPARE sequentialVertexColoring14 AS
SELECT *
FROM pgr_sequentialVertexColoring(
    'q13'
);

RETURN QUERY
SELECT set_eq('sequentialVertexColoring14', $$VALUES (1, 1), (2, 2), (3, 1), (4, 2)$$, '14: Two colors are required');


-- 4 vertices test (cyclic)

PREPARE q15 AS
SELECT id, source, target, cost, reverse_cost
FROM edge_table
WHERE id IN (8, 10, 11, 12);

RETURN QUERY
SELECT set_eq('q15',
    $$VALUES
        (8, 5, 6, 1, 1),
        (10, 5, 10, 1, 1),
        (11, 6, 11, 1, -1),
        (12, 10, 11, 1, -1)
    $$,
    'q15: Graph with four vertices 5, 6, 10 and 11 (cyclic)'
);

PREPARE sequentialVertexColoring16 AS
SELECT *
FROM pgr_sequentialVertexColoring(
    'q15'
);

RETURN QUERY
SELECT set_eq('sequentialVertexColoring16', $$VALUES (5, 1), (6, 2), (10, 2), (11, 1)$$, '16: Two colors are required');


-- 4 vertices test (all connected)

CREATE TABLE four_vertices_table (
    id BIGSERIAL,
    source BIGINT,
    target BIGINT,
    cost FLOAT,
    reverse_cost FLOAT
);

INSERT INTO four_vertices_table (source, target, cost, reverse_cost) VALUES
    (1, 2, 1, 1),
    (2, 3, 1, -1),
    (3, 4, -1, 1),
    (4, 1, 1, 1),
    (1, 3, 1, -1),
    (2, 4, -1, 1);

PREPARE q17 AS
SELECT id, source, target, cost, reverse_cost
FROM four_vertices_table;

RETURN QUERY
SELECT set_eq('q17',
    $$VALUES
        (1, 1, 2, 1, 1),
        (2, 2, 3, 1, -1),
        (3, 3, 4, -1, 1),
        (4, 4, 1, 1, 1),
        (5, 1, 3, 1, -1),
        (6, 2, 4, -1, 1)
    $$,
    'q17: Graph with four vertices 1, 2, 3 and 4 (all connected)'
);

PREPARE sequentialVertexColoring18 AS
SELECT *
FROM pgr_sequentialVertexColoring(
    'q17'
);

RETURN QUERY
SELECT set_eq('sequentialVertexColoring18', $$VALUES (1, 1), (2, 2), (3, 3), (4, 4)$$, '18: Four colors are required');

END;
$BODY$
LANGUAGE plpgsql;

SELECT edge_cases();


SELECT * FROM finish();
ROLLBACK;
