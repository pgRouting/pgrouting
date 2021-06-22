\i setup.sql

UPDATE edge_table SET cost = sign(cost), reverse_cost = sign(reverse_cost);
SELECT CASE WHEN min_version('3.2.0') THEN plan (18) ELSE plan(1) END;

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
SELECT is_empty('q1', '1: Graph with 0 edge and 0 vertex');

PREPARE depthFirstSearch2 AS
SELECT *
FROM pgr_depthFirstSearch(
    'q1',
    5, directed => false
);

RETURN QUERY
SELECT set_eq('depthFirstSearch2',
    $$VALUES (1, 0, 5, 5, -1, 0, 0)$$,
    '2: 0 edge 0 vertex test'
);



-- vertex not present in graph test

PREPARE q3 AS
SELECT id, source, target, cost, reverse_cost
FROM edge_table;

RETURN QUERY
SELECT is_empty(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table
    WHERE source = 100 OR target = 100',
    '3: Vertex 100 does not exist in sample data'
);

PREPARE depthFirstSearch4 AS
SELECT *
FROM pgr_depthFirstSearch(
    'q3',
    100, directed => false
);

RETURN QUERY
SELECT set_eq('depthFirstSearch4',
    $$VALUES (1, 0, 100, 100, -1, 0, 0)$$,
    '4: Vertex not present in graph'
);



-- negative depth test

PREPARE depthFirstSearch5 AS
SELECT *
FROM pgr_depthFirstSearch(
    'q3',
    4, directed => false, max_depth => -3
);

RETURN QUERY
SELECT throws_ok('depthFirstSearch5',
    'P0001', 'Negative value found on ''max_depth''',
    '5: Negative max_depth throws'
);



-- 1 vertex test

PREPARE q6 AS
SELECT id, source, 2 AS target, cost, reverse_cost
FROM edge_table
WHERE id = 2;

RETURN QUERY
SELECT set_eq('q6',
    $$VALUES (2, 2, 2, -1, 1)$$,
    '6: Graph with only vertex 2'
);

PREPARE depthFirstSearch7 AS
SELECT *
FROM pgr_depthFirstSearch(
    'q6',
    2, directed => false
);

RETURN QUERY
SELECT set_eq('depthFirstSearch7',
    $$VALUES (1, 0, 2, 2, -1, 0, 0)$$,
    '7: 1 vertex test'
);



-- 2 vertices tests

PREPARE q8 AS
SELECT id, source, target, cost, reverse_cost
FROM edge_table
WHERE id = 5;

RETURN QUERY
SELECT set_eq('q8',
    $$VALUES (5, 3, 6, 1, -1)$$,
    '8: Graph with two vertices 3 and 6 and edge from 3 to 6'
);

PREPARE depthFirstSearch9 AS
SELECT *
FROM pgr_depthFirstSearch(
    'q8',
    ARRAY[3, 6], directed => false
);

PREPARE depthFirstSearch10 AS
SELECT *
FROM pgr_depthFirstSearch(
    'q8',
    ARRAY[3, 6], directed => false, max_depth => 0
);

PREPARE depthFirstSearch11 AS
SELECT *
FROM pgr_depthFirstSearch(
    'q8',
    ARRAY[3, 6], directed => false, max_depth => 1
);

RETURN QUERY
SELECT set_eq('depthFirstSearch9',
    $$VALUES
        (1, 0, 3, 3, -1, 0, 0),
        (2, 1, 3, 6, 5, 1, 1),
        (3, 0, 6, 6, -1, 0, 0),
        (4, 1, 6, 3, 5, 1, 1)
    $$,
    '9: Two vertices tests'
);

RETURN QUERY
SELECT set_eq('depthFirstSearch10',
    $$VALUES
        (1, 0, 3, 3, -1, 0, 0),
        (2, 0, 6, 6, -1, 0, 0)
    $$,
    '10: Two vertices tests'
);

RETURN QUERY
SELECT set_eq('depthFirstSearch11',
    $$VALUES
        (1, 0, 3, 3, -1, 0, 0),
        (2, 1, 3, 6, 5, 1, 1),
        (3, 0, 6, 6, -1, 0, 0),
        (4, 1, 6, 3, 5, 1, 1)
    $$,
    '11: Two vertices tests'
);



-- 3 vertices tests

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

PREPARE q12 AS
SELECT id, source, target, cost, reverse_cost
FROM three_vertices_table;

-- Cyclic Graph with three vertices 3, 6 and 8
RETURN QUERY
SELECT set_eq('q12',
    $$VALUES
        (1, 3, 6, 20, 15),
        (2, 3, 8, 10, -10),
        (3, 6, 8, -1, 12)
    $$,
    '12: Cyclic Graph with three vertices 3, 6 and 8'
);

PREPARE depthFirstSearch13 AS
SELECT *
FROM pgr_depthFirstSearch(
    'q12',
    ARRAY[3, 6, 8], directed => false
);

PREPARE depthFirstSearch14 AS
SELECT *
FROM pgr_depthFirstSearch(
    'q12',
    ARRAY[3, 6, 8], directed => false, max_depth => 1
);


RETURN QUERY
SELECT set_eq('depthFirstSearch13',
    $$VALUES
        (1, 0, 3, 3, -1, 0, 0),
        (2, 1, 3, 6, 1, 20, 20),
        (3, 2, 3, 8, 3, 12, 32),
        (4, 0, 6, 6, -1, 0, 0),
        (5, 1, 6, 3, 1, 20, 20),
        (6, 2, 6, 8, 2, 10, 30),
        (7, 0, 8, 8, -1, 0, 0),
        (8, 1, 8, 3, 2, 10, 10),
        (9, 2, 8, 6, 1, 20, 30)
    $$,
    '13: 3 vertices tests'
);

RETURN QUERY
SELECT set_eq('depthFirstSearch14',
    $$VALUES
        (1, 0, 3, 3, -1, 0, 0),
        (2, 1, 3, 6, 1, 20, 20),
        (3, 1, 3, 8, 2, 10, 10),
        (4, 0, 6, 6, -1, 0, 0),
        (5, 1, 6, 3, 1, 20, 20),
        (6, 1, 6, 8, 3, 12, 12),
        (7, 0, 8, 8, -1, 0, 0),
        (8, 1, 8, 3, 2, 10, 10),
        (9, 1, 8, 6, 3, 12, 12)
    $$,
    '14: 3 vertices tests'
);



-- 4 vertices tests

PREPARE q15 AS
SELECT id, source, target, cost, reverse_cost
FROM edge_table
WHERE (id >= 10 AND id <= 12)
    OR id = 8;

RETURN QUERY
SELECT set_eq('q15',
    $$VALUES
        (8, 5, 6, 1, 1),
        (10, 5, 10, 1, 1),
        (11, 6, 11, 1, -1),
        (12, 10, 11, 1, -1)
    $$,
    '15: Graph with vertices 5, 6, 10 and 11'
);

PREPARE depthFirstSearch16 AS
SELECT *
FROM pgr_depthFirstSearch(
    'q15',
    ARRAY[5, 6, 10, 11], directed => false
);

PREPARE depthFirstSearch17 AS
SELECT *
FROM pgr_depthFirstSearch(
    'q15',
    ARRAY[5, 6, 10, 11], directed => false, max_depth => 1
);

PREPARE depthFirstSearch18 AS
SELECT *
FROM pgr_depthFirstSearch(
    'q15',
    ARRAY[5, 6, 10, 11], directed => false, max_depth => 2
);

RETURN QUERY
SELECT set_eq('depthFirstSearch16',
    $$VALUES
        (1, 0, 5, 5, -1, 0, 0),
        (2, 1, 5, 6, 8, 1, 1),
        (3, 2, 5, 11, 11, 1, 2),
        (4, 3, 5, 10, 12, 1, 3),
        (5, 0, 6, 6, -1, 0, 0),
        (6, 1, 6, 5, 8, 1, 1),
        (7, 2, 6, 10, 10, 1, 2),
        (8, 3, 6, 11, 12, 1, 3),
        (9, 0, 10, 10, -1, 0, 0),
        (10, 1, 10, 5, 10, 1, 1),
        (11, 2, 10, 6, 8, 1, 2),
        (12, 3, 10, 11, 11, 1, 3),
        (13, 0, 11, 11, -1, 0, 0),
        (14, 1, 11, 6, 11, 1, 1),
        (15, 2, 11, 5, 8, 1, 2),
        (16, 3, 11, 10, 10, 1, 3)
    $$,
    '16: 4 vertices tests'
);

RETURN QUERY
SELECT set_eq('depthFirstSearch17',
    $$VALUES
        (1, 0, 5, 5, -1, 0, 0),
        (2, 1, 5, 6, 8, 1, 1),
        (3, 1, 5, 10, 10, 1, 1),
        (4, 0, 6, 6, -1, 0, 0),
        (5, 1, 6, 5, 8, 1, 1),
        (6, 1, 6, 11, 11, 1, 1),
        (7, 0, 10, 10, -1, 0, 0),
        (8, 1, 10, 5, 10, 1, 1),
        (9, 1, 10, 11, 12, 1, 1),
        (10, 0, 11, 11, -1, 0, 0),
        (11, 1, 11, 6, 11, 1, 1),
        (12, 1, 11, 10, 12, 1, 1)
    $$,
    '17: 4 vertices tests'
);

RETURN QUERY
SELECT set_eq('depthFirstSearch18',
    $$VALUES
        (1, 0, 5, 5, -1, 0, 0),
        (2, 1, 5, 6, 8, 1, 1),
        (3, 2, 5, 11, 11, 1, 2),
        (4, 1, 5, 10, 10, 1, 1),
        (5, 0, 6, 6, -1, 0, 0),
        (6, 1, 6, 5, 8, 1, 1),
        (7, 2, 6, 10, 10, 1, 2),
        (8, 1, 6, 11, 11, 1, 1),
        (9, 0, 10, 10, -1, 0, 0),
        (10, 1, 10, 5, 10, 1, 1),
        (11, 2, 10, 6, 8, 1, 2),
        (12, 1, 10, 11, 12, 1, 1),
        (13, 0, 11, 11, -1, 0, 0),
        (14, 1, 11, 6, 11, 1, 1),
        (15, 2, 11, 5, 8, 1, 2),
        (16, 1, 11, 10, 12, 1, 1)
    $$,
    '18: 4 vertices tests'
);

END;
$BODY$
LANGUAGE plpgsql;

SELECT edge_cases();


SELECT * FROM finish();
ROLLBACK;
