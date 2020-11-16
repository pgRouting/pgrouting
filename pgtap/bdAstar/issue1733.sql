\i setup.sql

SELECT plan(3);

SELECT is_empty(
    'SELECT id, source, target, cost, reverse_cost, x1, y1, x2, y2
    FROM edge_table
    WHERE source = 100 OR target = 100',
    'Vertex 100 does not exist in sample data'
);

PREPARE q1 AS
SELECT * FROM pgr_bdAstar(
    'SELECT id, source, target, cost, reverse_cost, x1, y1, x2, y2
    FROM edge_table',
    2, 100,
    true, heuristic := 2
);

SELECT * FROM lives_ok('q1');
SELECT * FROM is_empty('q1');


SELECT finish();
