\i setup.sql

SELECT plan(30);



-- 0 edge, 0 vertex tests

PREPARE q1 AS
SELECT id, source, target, cost, reverse_cost
FROM edge_table
WHERE id > 18;

-- Graph is empty - it has 0 edge and 0 vertex
SELECT is_empty('q1', 'q1: Graph with 0 edge and 0 vertex');

-- 0 edge, 0 vertex tests

PREPARE boyerMyrvold2 AS
SELECT *
FROM pgr_boyerMyrvold(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table
    WHERE id > 18'
);

PREPARE boyerMyrvold3 AS
SELECT *
FROM pgr_boyerMyrvold(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table
    WHERE id > 18'
);

PREPARE boyerMyrvold AS
SELECT *
FROM pgr_boyerMyrvold(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table
    WHERE id > 18'
);

PREPARE boyerMyrvold5 AS
SELECT *
FROM pgr_boyerMyrvold(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table
    WHERE id > 18'
);

PREPARE boyerMyrvold6 AS
SELECT *
FROM pgr_boyerMyrvold(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table
    WHERE id > 18'
);

PREPARE boyerMyrvold7 AS
SELECT *
FROM pgr_boyerMyrvold(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table
    WHERE id > 18'
);

SELECT is_empty('boyerMyrvold2', '2: Graph with 0 edge and 0 vertex -> Empty row is returned');
SELECT is_empty('boyerMyrvold3', '3: Graph with 0 edge and 0 vertex -> Empty row is returned');
SELECT is_empty('boyerMyrvold', '4: Graph with 0 edge and 0 vertex -> Empty row is returned');
SELECT is_empty('boyerMyrvold5', '5: Graph with 0 edge and 0 vertex -> Empty row is returned');
SELECT is_empty('boyerMyrvold6', '6: Graph with 0 edge and 0 vertex -> Empty row is returned');
SELECT is_empty('boyerMyrvold7', '7: Graph with 0 edge and 0 vertex -> Empty row is returned');

-- vertex not present in graph tests

PREPARE boyerMyrvold8 AS
SELECT *
FROM pgr_boyerMyrvold(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table WHERE id = -10'
);

PREPARE boyerMyrvold9 AS
SELECT *
FROM pgr_boyerMyrvold(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table WHERE id IN (-10,50)'
);

PREPARE boyerMyrvold10 AS
SELECT *
FROM pgr_boyerMyrvold(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table WHERE id IN (-10,-20,-30)'
);

PREPARE boyerMyrvold11 AS
SELECT *
FROM pgr_boyerMyrvold(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table WHERE id IN (-10,33,39)'
);

PREPARE boyerMyrvold12 AS
SELECT *
FROM pgr_boyerMyrvold(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table WHERE id = 36'
);

PREPARE boyerMyrvold13 AS
SELECT *
FROM pgr_boyerMyrvold(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table WHERE id IN (36,45,34)'
);

SELECT is_empty('boyerMyrvold8', '8: Vertex not present in graph -> Empty row is returned');
SELECT is_empty('boyerMyrvold9', '9: Vertex not present in graph -> Empty row is returned');
SELECT is_empty('boyerMyrvold10', '10: Vertex not present in graph -> Empty row is returned');
SELECT is_empty('boyerMyrvold11', '11: Vertex not present in graph -> Empty row is returned');
SELECT is_empty('boyerMyrvold12', '12: Vertex not present in graph -> Empty row is returned');
SELECT is_empty('boyerMyrvold13', '13: Vertex not present in graph -> Empty row is returned');


-- 1 vertex tests

PREPARE q14 AS
SELECT id, source, 6 AS target, cost, reverse_cost
FROM edge_table
WHERE id = 9;

-- Graph with only vertex 9
SELECT set_eq('q14', $$VALUES (9, 6, 6, 1, 1)$$, 'q14: Graph with only vertex 6');

-- 1 vertex tests

PREPARE boyerMyrvold15 AS
SELECT *
FROM pgr_boyerMyrvold(
    'SELECT id, source, 6 AS target, cost, reverse_cost
    FROM edge_table
    WHERE id = 9'
);

PREPARE boyerMyrvold16 AS
SELECT *
FROM pgr_boyerMyrvold(
    'SELECT id, source, 6 AS target, cost, reverse_cost
    FROM edge_table
    WHERE id = 9'
);

PREPARE boyerMyrvold17 AS
SELECT *
FROM pgr_boyerMyrvold(
    'SELECT id, source, 6 AS target, cost, reverse_cost
    FROM edge_table
    WHERE id = 9'
);

PREPARE boyerMyrvold18 AS
SELECT *
FROM pgr_boyerMyrvold(
    'SELECT id, source, 6 AS target, cost, reverse_cost
    FROM edge_table
    WHERE id = 9'
);

SELECT set_eq('boyerMyrvold15', $$VALUES (1, 6, 6, 1)$$, '15: One row is returned');
SELECT set_eq('boyerMyrvold16', $$VALUES (1, 6, 6, 1)$$, '16: One row is returned');
SELECT set_eq('boyerMyrvold17', $$VALUES (1, 6, 6, 1)$$, '17: One row is returned');
SELECT set_eq('boyerMyrvold18', $$VALUES (1, 6, 6, 1)$$, '18: One row is returned');


-- 2 vertices tests

PREPARE boyerMyrvold19 AS
SELECT *
FROM pgr_boyerMyrvold(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table
    WHERE id = 1'
);

PREPARE boyerMyrvold20 AS
SELECT *
FROM pgr_boyerMyrvold(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table
    WHERE id = 4'
);

PREPARE boyerMyrvold21 AS
SELECT *
FROM pgr_boyerMyrvold(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table
    WHERE id = 9'
);

PREPARE boyerMyrvold22 AS
SELECT *
FROM pgr_boyerMyrvold(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table
    WHERE id = 6'
);

PREPARE boyerMyrvold23 AS
SELECT *
FROM pgr_boyerMyrvold(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table
    WHERE id = 7'
);

PREPARE boyerMyrvold24 AS
SELECT *
FROM pgr_boyerMyrvold(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table
    WHERE id = 8'
);

SELECT set_eq('boyerMyrvold19', $$VALUES (1, 1, 2, 1)$$, '19: Two rows are returned');
SELECT set_eq('boyerMyrvold20', $$VALUES (1, 2, 5, 1)$$, '20: Two rows are returned');
SELECT set_eq('boyerMyrvold21', $$VALUES (1, 6, 9, 1)$$, '21: Two rows are returned');
SELECT set_eq('boyerMyrvold22', $$VALUES (1, 7, 8, 1)$$, '22: Two rows are returned');
SELECT set_eq('boyerMyrvold23', $$VALUES (1, 8, 5, 1)$$, '23: Two rows are returned');
SELECT set_eq('boyerMyrvold24', $$VALUES (1, 5, 6, 1)$$, '24: Two rows are returned');



-- 3 vertices tests

CREATE TABLE three_vertices_table (
    id BIGSERIAL,
    source BIGINT,
    target BIGINT,
    cost FLOAT,
    reverse_cost FLOAT
);

INSERT INTO three_vertices_table (source, target, cost, reverse_cost) VALUES
    (3, 6, 20, -1),
    (3, 8, 10, -1),
    (6, 8, -1, 12);

PREPARE q25 AS
SELECT id, source, target, cost, reverse_cost
FROM three_vertices_table;

-- Cyclic Graph with three vertices 3, 6 and 8
SELECT set_eq('q25',
    $$VALUES
        (1, 3, 6, 20, -1),
        (2, 3, 8, 10, -1),
        (3, 6, 8, -1, 12)
    $$,
    'q25: Cyclic Graph with three vertices 3, 6 and 8'
);

-- 3 vertices tests

PREPARE boyerMyrvold26 AS
SELECT *
FROM pgr_boyerMyrvold(
    'SELECT id, source, target, cost, reverse_cost
    FROM three_vertices_table'
);

PREPARE boyerMyrvold27 AS
SELECT *
FROM pgr_boyerMyrvold(
    'SELECT id, source, target, cost, reverse_cost
    FROM three_vertices_table WHERE id > 4'
);

SELECT set_eq('boyerMyrvold26',
    $$VALUES
        (1, 3, 6, 20),
        (2, 3, 8, 10),
        (3, 8, 6, 12)
    $$,
    '26: 3 vertices tests'
);
SELECT is_empty('boyerMyrvold27',
    '27: Vertex not present in graph -> Empty row is returned'
);

-- 4 vertices tests

PREPARE q28 AS
SELECT id, source, target, cost, reverse_cost
FROM edge_table
WHERE (id >= 10 AND id <= 12)
    OR id = 8;

-- Graph with vertices 5, 6, 10, 11
SELECT set_eq('q28',
    $$VALUES
        (8, 5, 6, 1, 1),
        (10, 5, 10, 1, 1),
        (11, 6, 11, 1, -1),
        (12, 10, 11, 1, -1)
    $$,
    '28: Graph with vertices 5, 6, 10 and 11'
);

-- 4 vertices tests

PREPARE boyerMyrvold29 AS
SELECT *
FROM pgr_boyerMyrvold(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table
    WHERE (id >= 10 AND id <= 12)
        OR id = 8'
);

PREPARE boyerMyrvold30 AS
SELECT *
FROM pgr_boyerMyrvold(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table
    WHERE (id >= 2 AND id <= 4)'
);

SELECT set_eq('boyerMyrvold29',
    $$VALUES
        (1, 5, 6, 1),
        (2, 5, 10, 1),
        (3, 6, 11, 1),
        (4, 10, 11, 1)
    $$,
    '29: 4 vertices tests,4 rows returned'
);

SELECT set_eq('boyerMyrvold30',
    $$VALUES
        (1, 3, 2, 1),
        (2, 4, 3, 1),
        (3, 2, 5, 1),
    $$,
    '30: 4 vertices tests,3 rows returned'
);


SELECT * FROM finish();
ROLLBACK;
