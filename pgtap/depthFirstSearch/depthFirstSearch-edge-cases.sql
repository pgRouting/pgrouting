\i setup.sql

SELECT plan(6);

SELECT todo_start('Must add all edge cases');

-- 0 edge, 0 vertex (directed)

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

SELECT todo_end();

SELECT * FROM finish();
ROLLBACK;
