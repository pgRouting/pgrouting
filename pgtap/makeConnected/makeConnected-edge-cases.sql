\i setup.sql

SELECT plan(4);



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
FROM edge_table
WHERE id = -10;

SELECT is_empty('q3', 'q3: Vertex Not present in the graph');

PREPARE makeConnected4 AS
SELECT *
FROM pgr_makeConnected(
    'q4'
);

SELECT is_empty('makeConnected4', '4: Vertex Not present in the graph -> Empty row is returned');







SELECT * FROM finish();
ROLLBACK;
