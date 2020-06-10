\echo -- SAMPLE TABLE CREATE start
CREATE TABLE sample_table (
    id BIGSERIAL,
    source BIGINT,
    target BIGINT,
    cost FLOAT,
    reverse_cost FLOAT
);
\echo -- SAMPLE TABLE CREATE end
\echo

\echo -- SAMPLE TABLE ADD DATA start
INSERT INTO sample_table (source, target, cost, reverse_cost) VALUES
    (3, 6, 20, 15),
    (3, 8, 10, -10),
    (6, 8, -1, 12);
\echo -- SAMPLE TABLE ADD DATA end
\echo

-- SELECT query on SAMPLE TABLE
\echo -- q0
SELECT * FROM sample_table ORDER BY id;

-- Directed Graph with start_vid 3
\echo -- q1
SELECT * FROM pgr_depthFirstSearch (
    'SELECT id, source, target, cost, reverse_cost FROM sample_table ORDER BY id',
    3
);

-- Directed Graph with start_vid 6
\echo -- q2
SELECT * FROM pgr_depthFirstSearch (
    'SELECT id, source, target, cost, reverse_cost FROM sample_table ORDER BY id',
    6
);

-- Directed Graph with start_vid 6 and max_depth 1
\echo -- q3
SELECT * FROM pgr_depthFirstSearch (
    'SELECT id, source, target, cost, reverse_cost FROM sample_table ORDER BY id',
    6, max_depth => 1
);

-- start_vid does not exist in the Graph
\echo -- q4
SELECT * FROM pgr_depthFirstSearch (
    'SELECT id, source, target, cost, reverse_cost FROM sample_table ORDER BY id',
    2
);

-- Undirected Graph with start_vid 3
\echo -- q5
SELECT * FROM pgr_depthFirstSearch (
    'SELECT id, source, target, cost, reverse_cost FROM sample_table ORDER BY id',
    3, directed => false
);

-- Undirected Graph with start_vid 6
\echo -- q6
SELECT * FROM pgr_depthFirstSearch (
    'SELECT id, source, target, cost, reverse_cost FROM sample_table ORDER BY id',
    6, directed => false
);

-- Undirected Graph with start_vid 6 and max_depth 1
\echo -- q7
SELECT * FROM pgr_depthFirstSearch (
    'SELECT id, source, target, cost, reverse_cost FROM sample_table ORDER BY id',
    6, max_depth => 1, directed => false
);

-- Directed Graph with multiple start_vids
\echo -- q8
SELECT * FROM pgr_depthFirstSearch (
    'SELECT id, source, target, cost, reverse_cost FROM sample_table ORDER BY id',
    ARRAY[6, 3, 6]
);
\echo -- q9
