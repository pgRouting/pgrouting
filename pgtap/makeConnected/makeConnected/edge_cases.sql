\i setup.sql

SELECT plan(18);

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
FROM pgr_makeConnected(
    'q1'
);

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
FROM pgr_makeConnected(
    'q5'
);

SELECT is_empty('oneVertexTest6', '6: Graph is already Connected -> Empty row is returned');

-- 2 vertices tests ===> Already Connected

PREPARE makeConnected6 AS
SELECT *
FROM pgr_makeConnected(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table
    WHERE id = 1'
);

SELECT is_empty('makeConnected6', '6: Graph is already Connected -> Empty row is returned');
-- 2 vertices tests ===> Not Connected

PREPARE makeConnected7 AS
SELECT *
FROM pgr_makeConnected('SELECT id, source, 2 AS target, cost, reverse_cost
                              FROM edge_table WHERE id = 2
                                          UNION
                        SELECT id, source, 6 AS target, cost, reverse_cost
                              FROM edge_table WHERE id = 9'
);

SELECT set_eq('makeConnected7', $$VALUES (1, 2, 6)$$, '7: One row is returned');

PREPARE makeConnected8 AS
SELECT *
FROM pgr_makeConnected('SELECT id,  source, 7 AS target, cost, reverse_cost
                                      FROM edge_table WHERE id = 6
                                          UNION
                                SELECT id, source, 6 AS target, cost, reverse_cost
                                      FROM edge_table WHERE id = 9'
);
SELECT set_eq('makeConnected8', $$VALUES (1, 7, 6)$$, '8: One row is returned');


-- 3 vertices tests ====> Already Connnected

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

PREPARE q9 AS
SELECT id, source, target, cost, reverse_cost
FROM three_vertices_table;

-- Cyclic Graph with three vertices 3, 6 and 8
SELECT set_eq('q9',
    $$VALUES
        (1, 3, 6, 20, -1),
        (2, 3, 8, 10, -1),
        (3, 6, 8, -1, 12)
    $$,
    'q9: Cyclic Graph with three vertices 3, 6 and 8'
);


PREPARE makeConnected10 AS
SELECT *
FROM pgr_makeConnected(
    'SELECT id, source, target, cost, reverse_cost
    FROM three_vertices_table'
);

PREPARE makeConnected11 AS
SELECT *
FROM pgr_makeConnected(
    'SELECT id, source, target, cost, reverse_cost
    FROM three_vertices_table WHERE id > 4'
);

SELECT is_empty('makeConnected10',
    '10: Graph Already Connected -> Empty row is returned'
);
SELECT is_empty('makeConnected11',
    '11: Vertex not present in graph -> Empty row is returned'
);

-- 3 vertices tests ====> Not Connnected
PREPARE makeConnected12 AS
SELECT *
FROM pgr_makeConnected('SELECT id,  source, target, cost, reverse_cost
                                FROM edge_table WHERE id = 2
                                        UNION
                        SELECT id, source, 6 AS target, cost, reverse_cost
                                FROM edge_table WHERE id = 9'
);

PREPARE makeConnected13 AS
SELECT *
FROM pgr_makeConnected('SELECT id,  source, 2 AS target, cost, reverse_cost
                                FROM edge_table WHERE id = 2
                                    UNION
                        SELECT id, source, target, cost, reverse_cost
                                FROM edge_table WHERE id = 17'
);

SELECT set_eq('makeConnected12',
    $$VALUES
        (1, 3, 6)
    $$,
    '12:Graph with three vertices 1, 2 and 6'
);

SELECT set_eq('makeConnected13',
    $$VALUES
        (1, 2, 14)
    $$,
    '13:Graph with three vertices 2, 14 and 15'
);

-- 4 vertex tests ===> Already Connected

PREPARE makeConnected14 AS
SELECT *
FROM pgr_makeConnected(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table
    WHERE id IN (1,2)'
);

SELECT is_empty('makeConnected14', '14: Graph is already Connected -> Empty row is returned');

-- 4 vertex tests ===> Not Connected



PREPARE makeConnected15 AS
SELECT *
FROM pgr_makeConnected(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table
    WHERE id IN (1,6)'
);

PREPARE makeConnected16 AS
SELECT *
FROM pgr_makeConnected(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table
    WHERE id IN (2,9)'
);

PREPARE makeConnected17 AS
SELECT *
FROM pgr_makeConnected(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table
    WHERE id IN (17,18)'
);

SELECT set_eq('makeConnected15',
    $$VALUES
        (1, 2, 7)
    $$,
    '15:Two Connected Components. One row is returned'
);

SELECT set_eq('makeConnected16',
    $$VALUES
        (1, 3, 6)
    $$,
    '16:Two Connected Components. One row is returned'
);

SELECT set_eq('makeConnected17',
    $$VALUES
        (1, 15, 16)
    $$,
    '16:Two Connected Components. One row is returned'
);



SELECT * FROM finish();
ROLLBACK;
