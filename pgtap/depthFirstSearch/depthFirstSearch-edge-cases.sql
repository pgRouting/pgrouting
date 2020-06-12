\i setup.sql

SELECT plan(36);

SELECT todo_start('Must add all edge cases');

-- 0 edge, 0 vertex tests

PREPARE q1 AS
SELECT id, source, target, cost, reverse_cost
FROM edge_table
WHERE id > 18;

SELECT is_empty('q1', 'q1: Graph with 0 edge and 0 vertex');

-- 0 edge, 0 vertex tests (directed)

PREPARE depthFirstSearch1 AS
SELECT *
FROM pgr_depthFirstSearch(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table
    WHERE id > 18',
    5
);

PREPARE depthFirstSearch2 AS
SELECT *
FROM pgr_depthFirstSearch(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table
    WHERE id > 18',
    ARRAY[5]
);

PREPARE depthFirstSearch3 AS
SELECT *
FROM pgr_depthFirstSearch(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table
    WHERE id > 18',
    ARRAY[2, 5]
);

PREPARE depthFirstSearch4 AS
SELECT *
FROM pgr_depthFirstSearch(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table
    WHERE id > 18',
    5, max_depth => 2
);

PREPARE depthFirstSearch5 AS
SELECT *
FROM pgr_depthFirstSearch(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table
    WHERE id > 18',
    ARRAY[5], max_depth => 2
);

PREPARE depthFirstSearch6 AS
SELECT *
FROM pgr_depthFirstSearch(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table
    WHERE id > 18',
    ARRAY[2, 5], max_depth => 2
);

SELECT is_empty('depthFirstSearch1', '1: Graph with 0 edge and 0 vertex -> Empty row is returned');
SELECT is_empty('depthFirstSearch2', '2: Graph with 0 edge and 0 vertex -> Empty row is returned');
SELECT is_empty('depthFirstSearch3', '3: Graph with 0 edge and 0 vertex -> Empty row is returned');
SELECT is_empty('depthFirstSearch4', '4: Graph with 0 edge and 0 vertex -> Empty row is returned');
SELECT is_empty('depthFirstSearch5', '5: Graph with 0 edge and 0 vertex -> Empty row is returned');
SELECT is_empty('depthFirstSearch6', '6: Graph with 0 edge and 0 vertex -> Empty row is returned');

-- 0 edge, 0 vertex tests (undirected)

PREPARE depthFirstSearch7 AS
SELECT *
FROM pgr_depthFirstSearch(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table
    WHERE id > 18',
    5, directed => false
);

PREPARE depthFirstSearch8 AS
SELECT *
FROM pgr_depthFirstSearch(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table
    WHERE id > 18',
    ARRAY[5], directed => false
);

PREPARE depthFirstSearch9 AS
SELECT *
FROM pgr_depthFirstSearch(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table
    WHERE id > 18',
    ARRAY[2, 5], directed => false
);

PREPARE depthFirstSearch10 AS
SELECT *
FROM pgr_depthFirstSearch(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table
    WHERE id > 18',
    5, max_depth => 2, directed => false
);

PREPARE depthFirstSearch11 AS
SELECT *
FROM pgr_depthFirstSearch(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table
    WHERE id > 18',
    ARRAY[5], max_depth => 2, directed => false
);

PREPARE depthFirstSearch12 AS
SELECT *
FROM pgr_depthFirstSearch(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table
    WHERE id > 18',
    ARRAY[2, 5], max_depth => 2, directed => false
);

SELECT is_empty('depthFirstSearch7', '7: Graph with 0 edge and 0 vertex -> Empty row is returned');
SELECT is_empty('depthFirstSearch8', '8: Graph with 0 edge and 0 vertex -> Empty row is returned');
SELECT is_empty('depthFirstSearch9', '9: Graph with 0 edge and 0 vertex -> Empty row is returned');
SELECT is_empty('depthFirstSearch10', '10: Graph with 0 edge and 0 vertex -> Empty row is returned');
SELECT is_empty('depthFirstSearch11', '11: Graph with 0 edge and 0 vertex -> Empty row is returned');
SELECT is_empty('depthFirstSearch12', '12: Graph with 0 edge and 0 vertex -> Empty row is returned');

-- vertex not present in graph tests (directed)

PREPARE depthFirstSearch13 AS
SELECT *
FROM pgr_depthFirstSearch(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table',
    -10
);

PREPARE depthFirstSearch14 AS
SELECT *
FROM pgr_depthFirstSearch(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table',
    ARRAY[-10]
);

PREPARE depthFirstSearch15 AS
SELECT *
FROM pgr_depthFirstSearch(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table',
    ARRAY[20, -10]
);

PREPARE depthFirstSearch16 AS
SELECT *
FROM pgr_depthFirstSearch(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table',
    -10, max_depth => 2
);

PREPARE depthFirstSearch17 AS
SELECT *
FROM pgr_depthFirstSearch(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table',
    ARRAY[-10], max_depth => 2
);

PREPARE depthFirstSearch18 AS
SELECT *
FROM pgr_depthFirstSearch(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table',
    ARRAY[20, -10], max_depth => 2
);

SELECT is_empty('depthFirstSearch13', '13: Vertex not present in graph -> Empty row is returned');
SELECT is_empty('depthFirstSearch14', '14: Vertex not present in graph -> Empty row is returned');
SELECT is_empty('depthFirstSearch15', '15: Vertex not present in graph -> Empty row is returned');
SELECT is_empty('depthFirstSearch16', '16: Vertex not present in graph -> Empty row is returned');
SELECT is_empty('depthFirstSearch17', '17: Vertex not present in graph -> Empty row is returned');
SELECT is_empty('depthFirstSearch18', '18: Vertex not present in graph -> Empty row is returned');

-- vertex not present in graph tests (undirected)

PREPARE depthFirstSearch19 AS
SELECT *
FROM pgr_depthFirstSearch(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table',
    -10, directed => false
);

PREPARE depthFirstSearch20 AS
SELECT *
FROM pgr_depthFirstSearch(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table',
    ARRAY[-10], directed => false
);

PREPARE depthFirstSearch21 AS
SELECT *
FROM pgr_depthFirstSearch(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table',
    ARRAY[20, -10], directed => false
);

PREPARE depthFirstSearch22 AS
SELECT *
FROM pgr_depthFirstSearch(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table',
    -10, max_depth => 2, directed => false
);

PREPARE depthFirstSearch23 AS
SELECT *
FROM pgr_depthFirstSearch(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table',
    ARRAY[-10], max_depth => 2, directed => false
);

PREPARE depthFirstSearch24 AS
SELECT *
FROM pgr_depthFirstSearch(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table',
    ARRAY[20, -10], max_depth => 2, directed => false
);

SELECT is_empty('depthFirstSearch19', '19: Vertex not present in graph -> Empty row is returned');
SELECT is_empty('depthFirstSearch20', '20: Vertex not present in graph -> Empty row is returned');
SELECT is_empty('depthFirstSearch21', '21: Vertex not present in graph -> Empty row is returned');
SELECT is_empty('depthFirstSearch22', '22: Vertex not present in graph -> Empty row is returned');
SELECT is_empty('depthFirstSearch23', '23: Vertex not present in graph -> Empty row is returned');
SELECT is_empty('depthFirstSearch24', '24: Vertex not present in graph -> Empty row is returned');

-- negative depth tests

PREPARE depthFirstSearch25 AS
SELECT *
FROM pgr_depthFirstSearch(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table',
    4, max_depth => -3
);

PREPARE depthFirstSearch26 AS
SELECT *
FROM pgr_depthFirstSearch(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table',
    ARRAY[4], max_depth => -3
);

PREPARE depthFirstSearch27 AS
SELECT *
FROM pgr_depthFirstSearch(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table',
    ARRAY[4, 20], max_depth => -3
);

PREPARE depthFirstSearch28 AS
SELECT *
FROM pgr_depthFirstSearch(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table',
    4, max_depth => -3, directed => false
);

PREPARE depthFirstSearch29 AS
SELECT *
FROM pgr_depthFirstSearch(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table',
    ARRAY[4], max_depth => -3, directed => false
);

PREPARE depthFirstSearch30 AS
SELECT *
FROM pgr_depthFirstSearch(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table',
    ARRAY[4, 20], max_depth => -3, directed => false
);

SELECT throws_ok('depthFirstSearch25', 'P0001', 'Negative value found on ''max_depth''', '25: Negative max_depth throws');
SELECT throws_ok('depthFirstSearch26', 'P0001', 'Negative value found on ''max_depth''', '26: Negative max_depth throws');
SELECT throws_ok('depthFirstSearch27', 'P0001', 'Negative value found on ''max_depth''', '27: Negative max_depth throws');
SELECT throws_ok('depthFirstSearch28', 'P0001', 'Negative value found on ''max_depth''', '28: Negative max_depth throws');
SELECT throws_ok('depthFirstSearch29', 'P0001', 'Negative value found on ''max_depth''', '29: Negative max_depth throws');
SELECT throws_ok('depthFirstSearch30', 'P0001', 'Negative value found on ''max_depth''', '30: Negative max_depth throws');

-- 1 vertex tests

PREPARE q2 AS
SELECT id, source, 2 AS target, cost, reverse_cost
FROM edge_table
WHERE id = 2;

SELECT set_eq('q2', $$VALUES (2, 2, 2, -1, 1)$$, 'q2: Graph with only vertex 2');

-- 1 vertex tests (directed)

PREPARE depthFirstSearch31 AS
SELECT *
FROM pgr_depthFirstSearch(
    'SELECT id, source, 2 AS target, cost, reverse_cost
    FROM edge_table
    WHERE id = 2',
    2
);

PREPARE depthFirstSearch32 AS
SELECT *
FROM pgr_depthFirstSearch(
    'SELECT id, source, 2 AS target, cost, reverse_cost
    FROM edge_table
    WHERE id = 2',
    ARRAY[2]
);

PREPARE depthFirstSearch33 AS
SELECT *
FROM pgr_depthFirstSearch(
    'SELECT id, source, 2 AS target, cost, reverse_cost
    FROM edge_table
    WHERE id = 2',
    2, max_depth => 2
);

PREPARE depthFirstSearch34 AS
SELECT *
FROM pgr_depthFirstSearch(
    'SELECT id, source, 2 AS target, cost, reverse_cost
    FROM edge_table
    WHERE id = 2',
    ARRAY[2], max_depth => 2
);

SELECT set_eq('depthFirstSearch31', $$VALUES (1, 0, 2, 2, -1, 0, 0)$$, '31: One row with node 2 is returned');
SELECT set_eq('depthFirstSearch32', $$VALUES (1, 0, 2, 2, -1, 0, 0)$$, '32: One row with node 2 is returned');
SELECT set_eq('depthFirstSearch33', $$VALUES (1, 0, 2, 2, -1, 0, 0)$$, '33: One row with node 2 is returned');
SELECT set_eq('depthFirstSearch34', $$VALUES (1, 0, 2, 2, -1, 0, 0)$$, '34: One row with node 2 is returned');


SELECT todo_end();

SELECT * FROM finish();
ROLLBACK;
