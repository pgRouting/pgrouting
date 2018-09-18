\i setup.sql

SELECT plan(84);

PREPARE edges AS
SELECT id, source, target, cost, reverse_cost  FROM edge_table;

PREPARE pois AS
SELECT pid, edge_id, fraction from pointsOfInterest;

PREPARE null_ret AS
SELECT id FROM edge_table_vertices_pgr  WHERE id IN (-1);

PREPARE null_ret_arr AS
SELECT array_agg(id) FROM edge_table_vertices_pgr  WHERE id IN (-1);

SELECT isnt_empty('edges', 'Should be not empty to tests be meaningful');
SELECT isnt_empty('pois', 'Should be not empty to tests be meaningful');
SELECT is_empty('null_ret', 'Should be empty to tests be meaningful');
SELECT set_eq('null_ret_arr', 'SELECT NULL::BIGINT[]', 'Should be empty to tests be meaningful');


CREATE OR REPLACE FUNCTION test_function()
RETURNS SETOF TEXT AS
$BODY$
DECLARE
params TEXT[];
subs TEXT[];
BEGIN
    -- one to one
    params = ARRAY[
    '$$edges$$',
    '$$SELECT pid, edge_id, fraction from pointsOfInterest$$',
    '1::BIGINT',
    '2::BIGINT'
    ]::TEXT[];
    subs = ARRAY[
    'NULL',
    'NULL',
    '(SELECT id FROM edge_table_vertices_pgr  WHERE id IN (-1))',
    '(SELECT id FROM edge_table_vertices_pgr  WHERE id IN (-1))'
    ]::TEXT[];

    RETURN query SELECT * FROM no_crash_test('pgr_withPoints', params, subs);

    subs = ARRAY[
    'NULL',
    'NULL',
    'NULL::BIGINT',
    'NULL::BIGINT'
    ]::TEXT[];
    RETURN query SELECT * FROM no_crash_test('pgr_withPoints', params, subs);

    -- one to many
    params = ARRAY['$$edges$$',
    '$$SELECT pid, edge_id, fraction from pointsOfInterest$$',
    '1', 'ARRAY[2,5]::BIGINT[]']::TEXT[];
    subs = ARRAY[
    'NULL',
    'NULL',
    '(SELECT id FROM edge_table_vertices_pgr  WHERE id IN (-1))',
    '(SELECT array_agg(id) FROM edge_table_vertices_pgr  WHERE id IN (-1))'
    ]::TEXT[];

    RETURN query SELECT * FROM no_crash_test('pgr_withPoints', params, subs);

    subs = ARRAY[
    'NULL',
    'NULL',
    'NULL::BIGINT',
    'NULL::BIGINT[]'
    ]::TEXT[];
    RETURN query SELECT * FROM no_crash_test('pgr_withPoints', params, subs);

    -- many to one
    params = ARRAY['$$edges$$',
    '$$SELECT pid, edge_id, fraction from pointsOfInterest$$',
    'ARRAY[2,5]::BIGINT[]', '1']::TEXT[];
    subs = ARRAY[
    'NULL',
    'NULL',
    '(SELECT array_agg(id) FROM edge_table_vertices_pgr  WHERE id IN (-1))',
    '(SELECT id FROM edge_table_vertices_pgr  WHERE id IN (-1))'
    ]::TEXT[];

    RETURN query SELECT * FROM no_crash_test('pgr_withPoints', params, subs);

    subs = ARRAY[
    'NULL',
    'NULL',
    'NULL::BIGINT[]',
    'NULL::BIGINT'
    ]::TEXT[];
    RETURN query SELECT * FROM no_crash_test('pgr_withPoints', params, subs);

    -- many to many
    params = ARRAY['$$edges$$',
    '$$SELECT pid, edge_id, fraction from pointsOfInterest$$',
    'ARRAY[1]::BIGINT[]', 'ARRAY[2,5]::BIGINT[]']::TEXT[];
    subs = ARRAY[
    'NULL',
    'NULL',
    '(SELECT array_agg(id) FROM edge_table_vertices_pgr  WHERE id IN (-1))',
    '(SELECT array_agg(id) FROM edge_table_vertices_pgr  WHERE id IN (-1))'
    ]::TEXT[];

    RETURN query SELECT * FROM no_crash_test('pgr_withPoints', params, subs);

    subs = ARRAY[
    'NULL',
    'NULL',
    'NULL::BIGINT[]',
    'NULL::BIGINT[]'
    ]::TEXT[];
    RETURN query SELECT * FROM no_crash_test('pgr_withPoints', params, subs);

END
$BODY$
LANGUAGE plpgsql VOLATILE;


SELECT * FROM test_function();

ROLLBACK;
