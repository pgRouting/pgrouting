\i setup.sql

SELECT plan(7);



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

SELECT * FROM finish();
ROLLBACK;
