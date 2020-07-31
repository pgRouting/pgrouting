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

PREPARE boyerMyrvold_12 AS
SELECT *
FROM pgr_boyerMyrvold(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table
    WHERE id > 18'
);

PREPARE boyerMyrvold_13 AS
SELECT *
FROM pgr_boyerMyrvold(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table
    WHERE id IN (20,22)'
);

PREPARE boyerMyrvold_14 AS
SELECT *
FROM pgr_boyerMyrvold(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table
    WHERE id > 25'
);

PREPARE boyerMyrvold_15 AS
SELECT *
FROM pgr_boyerMyrvold(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table
    WHERE id IN (40,60,-10)'
);

PREPARE boyerMyrvold_16 AS
SELECT *
FROM pgr_boyerMyrvold(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table
    WHERE id > 18'
);

PREPARE boyerMyrvold_17 AS
SELECT *
FROM pgr_boyerMyrvold(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table
    WHERE id > 22'
);

SELECT is_empty('boyerMyrvold_12', '2: Graph with 0 edge and 0 vertex -> Empty row is returned');
SELECT is_empty('boyerMyrvold_13', '3: Graph with 0 edge and 0 vertex -> Empty row is returned');
SELECT is_empty('boyerMyrvold_14', '4: Graph with 0 edge and 0 vertex -> Empty row is returned');
SELECT is_empty('boyerMyrvold_15', '5: Graph with 0 edge and 0 vertex -> Empty row is returned');
SELECT is_empty('boyerMyrvold_16', '6: Graph with 0 edge and 0 vertex -> Empty row is returned');
SELECT is_empty('boyerMyrvold_17', '7: Graph with 0 edge and 0 vertex -> Empty row is returned');

-- vertex not present in graph tests

PREPARE boyerMyrvold_18 AS
SELECT *
FROM pgr_boyerMyrvold(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table WHERE id = -10'
);

PREPARE boyerMyrvold_19 AS
SELECT *
FROM pgr_boyerMyrvold(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table WHERE id IN (-10,50)'
);

PREPARE boyerMyrvold_110 AS
SELECT *
FROM pgr_boyerMyrvold(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table WHERE id IN (-10,-20,-30)'
);

PREPARE boyerMyrvold_111 AS
SELECT *
FROM pgr_boyerMyrvold(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table WHERE id IN (-10,33,39)'
);

PREPARE boyerMyrvold_112 AS
SELECT *
FROM pgr_boyerMyrvold(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table WHERE id = 36'
);

PREPARE boyerMyrvold_113 AS
SELECT *
FROM pgr_boyerMyrvold(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table WHERE id IN (36,45,34)'
);

SELECT is_empty('boyerMyrvold_18', '8: Vertex not present in graph -> Empty row is returned');
SELECT is_empty('boyerMyrvold_19', '9: Vertex not present in graph -> Empty row is returned');
SELECT is_empty('boyerMyrvold_110', '10: Vertex not present in graph -> Empty row is returned');
SELECT is_empty('boyerMyrvold_111', '11: Vertex not present in graph -> Empty row is returned');
SELECT is_empty('boyerMyrvold_112', '12: Vertex not present in graph -> Empty row is returned');
SELECT is_empty('boyerMyrvold_113', '13: Vertex not present in graph -> Empty row is returned');


-- 1 vertex tests

PREPARE q14 AS
SELECT id, source, 6 AS target, cost, reverse_cost
FROM edge_table
WHERE id = 9;

-- Graph with only vertex 9
SELECT set_eq('q14', $$VALUES (9, 6, 6, 1, 1)$$, 'q14: Graph with only vertex 6');

-- 1 vertex tests

PREPARE boyerMyrvold_115 AS
SELECT *
FROM pgr_boyerMyrvold(
    'SELECT id, source, 6 AS target, cost, reverse_cost
    FROM edge_table
    WHERE id = 9'
);

PREPARE boyerMyrvold_116 AS
SELECT *
FROM pgr_boyerMyrvold(
    'SELECT id, source, 2 AS target, cost, reverse_cost
    FROM edge_table
    WHERE id = 2'
);

PREPARE boyerMyrvold_117 AS
SELECT *
FROM pgr_boyerMyrvold(
    'SELECT id, source, 3 AS target, cost, reverse_cost
    FROM edge_table
    WHERE id = 3'
);

PREPARE boyerMyrvold_118 AS
SELECT *
FROM pgr_boyerMyrvold(
    'SELECT id, source, 7 AS target, cost, reverse_cost
    FROM edge_table
    WHERE id = 6'
);

SELECT set_eq('boyerMyrvold_115', $$VALUES (1, 6, 6, 1)$$, '15: One row is returned');
SELECT set_eq('boyerMyrvold_116', $$VALUES (1, 2, 2, 1)$$, '16: One row is returned');
SELECT set_eq('boyerMyrvold_117', $$VALUES (1, 3, 3, 1)$$, '17: One row is returned');
SELECT set_eq('boyerMyrvold_118', $$VALUES (1, 7, 7, 1)$$, '18: One row is returned');


-- 2 vertices tests

PREPARE boyerMyrvold_119 AS
SELECT *
FROM pgr_boyerMyrvold(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table
    WHERE id = 1'
);

PREPARE boyerMyrvold_120 AS
SELECT *
FROM pgr_boyerMyrvold(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table
    WHERE id = 4'
);

PREPARE boyerMyrvold_121 AS
SELECT *
FROM pgr_boyerMyrvold(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table
    WHERE id = 9'
);

PREPARE boyerMyrvold_122 AS
SELECT *
FROM pgr_boyerMyrvold(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table
    WHERE id = 6'
);

PREPARE boyerMyrvold_123 AS
SELECT *
FROM pgr_boyerMyrvold(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table
    WHERE id = 7'
);

PREPARE boyerMyrvold_124 AS
SELECT *
FROM pgr_boyerMyrvold(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table
    WHERE id = 8'
);

SELECT set_eq('boyerMyrvold_119', $$VALUES (1, 1, 2, 1)$$, '19: Two rows are returned');
SELECT set_eq('boyerMyrvold_120', $$VALUES (1, 2, 5, 1)$$, '20: Two rows are returned');
SELECT set_eq('boyerMyrvold_121', $$VALUES (1, 6, 9, 1)$$, '21: Two rows are returned');
SELECT set_eq('boyerMyrvold_122', $$VALUES (1, 7, 8, 1)$$, '22: Two rows are returned');
SELECT set_eq('boyerMyrvold_123', $$VALUES (1, 8, 5, 1)$$, '23: Two rows are returned');
SELECT set_eq('boyerMyrvold_124', $$VALUES (1, 5, 6, 1)$$, '24: Two rows are returned');



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

PREPARE boyerMyrvold_126 AS
SELECT *
FROM pgr_boyerMyrvold(
    'SELECT id, source, target, cost, reverse_cost
    FROM three_vertices_table'
);

PREPARE boyerMyrvold_127 AS
SELECT *
FROM pgr_boyerMyrvold(
    'SELECT id, source, target, cost, reverse_cost
    FROM three_vertices_table WHERE id > 4'
);

SELECT set_eq('boyerMyrvold_126',
    $$VALUES
        (1, 3, 6, 20),
        (2, 3, 8, 10),
        (3, 8, 6, 12)
    $$,
    '26: 3 vertices tests'
);
SELECT is_empty('boyerMyrvold_127',
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

PREPARE boyerMyrvold_129 AS
SELECT *
FROM pgr_boyerMyrvold(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table
    WHERE (id >= 10 AND id <= 12)
        OR id = 8'
);

PREPARE boyerMyrvold_130 AS
SELECT *
FROM pgr_boyerMyrvold(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table
    WHERE (id >= 2 AND id <= 4)'
);

SELECT set_eq('boyerMyrvold_129',
    $$VALUES
        (1, 5, 6, 1),
        (2, 5, 10, 1),
        (3, 6, 11, 1),
        (4, 10, 11, 1)
    $$,
    '29: 4 vertices tests,4 rows returned'
);

SELECT set_eq('boyerMyrvold_130',
    $$VALUES
        (1, 3, 2, 1),
        (2, 4, 3, 1),
        (3, 2, 5, 1)
    $$,
    '30: 4 vertices tests,3 rows returned'
);


SELECT * FROM finish();
ROLLBACK;
