\i setup.sql

SELECT plan(26);

PREPARE edges_xy AS
SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost, x1, y1, x2, y2  FROM edge_table;

PREPARE null_ret AS
SELECT id FROM edge_table_vertices_pgr  WHERE id IN (-1);

SELECT isnt_empty('edges_xy', 'Should be not empty to tests be meaningful');
SELECT is_empty('null_ret', 'Should be empty to tests be meaningful');


CREATE OR REPLACE FUNCTION test_function()
RETURNS SETOF TEXT AS
$BODY$
DECLARE
params TEXT[];
subs TEXT[];
BEGIN
    params = ARRAY['$$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost, x1, y1, x2, y2  FROM edge_table$$',
        '1', '2', 'true', 'true']::TEXT[];
    subs = ARRAY[
    'NULL',
    '(SELECT id::INTEGER FROM edge_table_vertices_pgr  WHERE id IN (-1))',
    '(SELECT id::INTEGER FROM edge_table_vertices_pgr  WHERE id IN (-1))',
    'NULL',
    'NULL'
    ]::TEXT[];

    RETURN query SELECT * FROM no_crash_test('pgr_bddijkstra', params, subs);

    subs = ARRAY[
    'NULL',
    'NULL',
    'NULL',
    'NULL',
    'NULL'
    ]::TEXT[];
    RETURN query SELECT * FROM no_crash_test('pgr_bddijkstra', params, subs);

END
$BODY$
LANGUAGE plpgsql VOLATILE;


SELECT * FROM test_function();

ROLLBACK;
