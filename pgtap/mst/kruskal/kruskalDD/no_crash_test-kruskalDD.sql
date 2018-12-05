\i setup.sql

SELECT plan(114);

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
    -- kruskalDD
    params = ARRAY[
    '$$SELECT id, source, target, cost, reverse_cost  FROM edge_table$$',
    '5',
    '3.5'
    ]::TEXT[];

    subs = ARRAY[
    'NULL',
    '(SELECT id FROM edge_table_vertices_pgr  WHERE id IN (-1))',
    'NULL'
    ]::TEXT[];

    RETURN query SELECT * FROM no_crash_test('pgr_kruskalDD', params, subs);

    params[1] := '$$edges$$';
    RETURN query SELECT * FROM no_crash_test('pgr_kruskalDD', params, subs);

    -- kruskalDD Multiple vertices
    params = ARRAY[
    '$$SELECT id, source, target, cost, reverse_cost  FROM edge_table$$',
    'ARRAY[5,3]',
    '3.5'
    ]::TEXT[];

    subs = ARRAY[
    'NULL',
    '(SELECT array_agg(id) FROM edge_table_vertices_pgr  WHERE id IN (-1))',
    'NULL'
    ]::TEXT[];

    RETURN query SELECT * FROM no_crash_test('pgr_kruskalDD', params, subs);

    params[1] := '$$edges$$';
    RETURN query SELECT * FROM no_crash_test('pgr_kruskalDD', params, subs);

    subs[2] := 'NULL::BIGINT[]';
    RETURN query SELECT * FROM no_crash_test('pgr_kruskalDD', params, subs);

    -- kruskalDD
    params = ARRAY[
    '$$SELECT id, source, target, cost, reverse_cost  FROM edge_table$$',
    '5',
    '3.5::numeric'
    ]::TEXT[];

    subs = ARRAY[
    'NULL',
    '(SELECT id FROM edge_table_vertices_pgr  WHERE id IN (-1))',
    'NULL::numeric'
    ]::TEXT[];
    RETURN query SELECT * FROM no_crash_test('pgr_kruskalDD', params, subs);

    params[3] := '3.5::float';
    RETURN query SELECT * FROM no_crash_test('pgr_kruskalDD', params, subs);

    params[1] := '$$edges$$';
    RETURN query SELECT * FROM no_crash_test('pgr_kruskalDD', params, subs);

    params[3] := '3.5::numeric';
    RETURN query SELECT * FROM no_crash_test('pgr_kruskalDD', params, subs);

    -- kruskalDD Multiple vertices
    params = ARRAY[
    '$$SELECT id, source, target, cost, reverse_cost  FROM edge_table$$',
    'ARRAY[5,3]',
    '3.5::numeric'
    ]::TEXT[];

    subs = ARRAY[
    'NULL',
    '(SELECT array_agg(id) FROM edge_table_vertices_pgr  WHERE id IN (-1))',
    'NULL::numeric'
    ]::TEXT[];
    RETURN query SELECT * FROM no_crash_test('pgr_kruskalDD', params, subs);

    params[3] := '3.5::float';
    RETURN query SELECT * FROM no_crash_test('pgr_kruskalDD', params, subs);

    params[1] := '$$edges$$';
    RETURN query SELECT * FROM no_crash_test('pgr_kruskalDD', params, subs);

    params[3] := '3.5::numeric';
    RETURN query SELECT * FROM no_crash_test('pgr_kruskalDD', params, subs);

    subs[2] := 'NULL::BIGINT[]';
    RETURN query SELECT * FROM no_crash_test('pgr_kruskalDD', params, subs);

END
$BODY$
LANGUAGE plpgsql VOLATILE;


SELECT * FROM test_function();

ROLLBACK;
