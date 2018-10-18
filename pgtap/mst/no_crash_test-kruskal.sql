\i setup.sql

SELECT plan(44);

PREPARE edges AS
SELECT id, source, target, cost, reverse_cost  FROM edge_table;

PREPARE null_vertex AS
SELECT id FROM edge_table_vertices_pgr  WHERE id IN (-1);


SELECT isnt_empty('edges', 'Should be not empty to tests be meaningful');
SELECT is_empty('null_vertex', 'Should be empty to tests be meaningful');


CREATE OR REPLACE FUNCTION test_function()
RETURNS SETOF TEXT AS
$BODY$
DECLARE
params TEXT[];
subs TEXT[];
BEGIN
    -- kruskal with no root vertex
    params = ARRAY[
    '$$SELECT id, source, target, cost, reverse_cost  FROM edge_table$$',
    '$$BFS$$::TEXT'
    ]::TEXT[];
    subs = ARRAY[
    'NULL',
    'NULL'
    ]::TEXT[];

    RETURN query SELECT * FROM no_crash_test('pgr_kruskal', params, subs);

    params[1] := '$$edges$$';
    RETURN query SELECT * FROM no_crash_test('pgr_kruskal', params, subs);

    params[2] := '$$DFS$$';
    RETURN query SELECT * FROM no_crash_test('pgr_kruskal', params, subs);


    -- kruskal with root vertex
    params = ARRAY[
    '$$SELECT id, source, target, cost, reverse_cost  FROM edge_table$$',
    '5',
    '$$BFS$$::TEXT'
    ]::TEXT[];

    subs = ARRAY[
    'NULL',
    '(SELECT id FROM edge_table_vertices_pgr  WHERE id IN (-1))',
    'NULL'
    ]::TEXT[];

    RETURN query SELECT * FROM no_crash_test('pgr_kruskal', params, subs);

    params[1] := '$$edges$$';
    RETURN query SELECT * FROM no_crash_test('pgr_kruskal', params, subs);

    params[3] := '$$DFS$$';
    RETURN query SELECT * FROM no_crash_test('pgr_kruskal', params, subs);

END
$BODY$
LANGUAGE plpgsql VOLATILE;


SELECT * FROM test_function();

ROLLBACK;
