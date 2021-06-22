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

SELECT CASE
WHEN NOT min_lib_version('3.1.1') THEN
  skip(2, 'Issue fixed on 3.1.1')
ELSE
  collect_tap(
    lives_ok('q1'),
    is_empty('q1'))
END;



SELECT finish();
