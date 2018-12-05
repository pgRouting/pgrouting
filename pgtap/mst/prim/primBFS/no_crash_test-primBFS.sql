\i setup.sql

SELECT plan(86);

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
    -- primBFS
    params = ARRAY[
    '$$SELECT id, source, target, cost, reverse_cost  FROM edge_table$$',
    '5'
    ]::TEXT[];
    subs = ARRAY[
    'NULL',
    '(SELECT id FROM edge_table_vertices_pgr  WHERE id IN (-1))'
    ]::TEXT[];

    RETURN query SELECT * FROM no_crash_test('pgr_primBFS', params, subs);

    params[1] := '$$edges$$';
    RETURN query SELECT * FROM no_crash_test('pgr_primBFS', params, subs);

    subs[2] := 'NULL::INTEGER';
    RETURN query SELECT * FROM no_crash_test('pgr_primBFS', params, subs);

    -- primBFS with depth
    params = ARRAY[
    '$$SELECT id, source, target, cost, reverse_cost  FROM edge_table$$',
    '5',
    '3'
    ]::TEXT[];
    subs = ARRAY[
    'NULL',
    '(SELECT id FROM edge_table_vertices_pgr  WHERE id IN (-1))',
    'NULL::INTEGER'
    ]::TEXT[];

    RETURN query SELECT * FROM no_crash_test('pgr_primBFS', params, subs);

    params[1] := '$$edges$$';
    RETURN query SELECT * FROM no_crash_test('pgr_primBFS', params, subs);

    subs[2] := 'NULL::BIGINT';
    RETURN query SELECT * FROM no_crash_test('pgr_primBFS', params, subs);

    -- primBFS Multiple vertices
    params = ARRAY[
    '$$SELECT id, source, target, cost, reverse_cost  FROM edge_table$$',
    'ARRAY[5,3]'
    ]::TEXT[];
    subs = ARRAY[
    'NULL',
    '(SELECT array_agg(id) FROM edge_table_vertices_pgr  WHERE id IN (-1))'
    ]::TEXT[];

    RETURN query SELECT * FROM no_crash_test('pgr_primBFS', params, subs);

    params[1] := '$$edges$$';
    RETURN query SELECT * FROM no_crash_test('pgr_primBFS', params, subs);

    subs[2] := 'NULL::BIGINT';
    RETURN query SELECT * FROM no_crash_test('pgr_primBFS', params, subs);

    -- primBFS with depth Multiple vertices
    params = ARRAY[
    '$$SELECT id, source, target, cost, reverse_cost  FROM edge_table$$',
    'ARRAY[5,3]',
    '3'
    ]::TEXT[];
    subs = ARRAY[
    'NULL',
    '(SELECT array_agg(id) FROM edge_table_vertices_pgr  WHERE id IN (-1))',
    'NULL::INTEGER'
    ]::TEXT[];

    RETURN query SELECT * FROM no_crash_test('pgr_primBFS', params, subs);

    params[1] := '$$edges$$';
    RETURN query SELECT * FROM no_crash_test('pgr_primBFS', params, subs);

    subs[2] := 'NULL::BIGINT[]';
    RETURN query SELECT * FROM no_crash_test('pgr_primBFS', params, subs);

END
$BODY$
LANGUAGE plpgsql VOLATILE;


SELECT * FROM test_function();

ROLLBACK;
