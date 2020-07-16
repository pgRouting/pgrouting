\i setup.sql

SELECT plan(8);



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
    'q3'
);

SELECT is_empty('makeConnected4', '4: Vertex Not present in the graph -> Empty row is returned');

-- 1 vertex tests

PREPARE makeConnected5 AS
SELECT *
FROM pgr_makeConnected(
    'SELECT id, source, 6 AS target, cost, reverse_cost
    FROM edge_table
    WHERE id = 9'
);

SELECT is_empty('makeConnected5', '5: Graph is already Connected -> Empty row is returned');

-- 2 vertices tests ===> Connected

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
SELECT * FROM pgr_makeConnected('SELECT id,  source, 7 AS target, cost, reverse_cost
                                      FROM edge_table WHERE id = 6
                                          UNION
                                SELECT id, source, 6 AS target, cost, reverse_cost
                                      FROM edge_table WHERE id = 9'
);
SELECT set_eq('makeConnected8', $$VALUES (1, 7, 6)$$, '8: One row is returned');






SELECT * FROM finish();
ROLLBACK;
