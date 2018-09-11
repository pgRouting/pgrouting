\i setup.sql

SELECT plan(27);

PREPARE edges AS
SELECT id, source, target, cost, reverse_cost  FROM edge_table;

PREPARE null_ret AS
SELECT id FROM edge_table_vertices_pgr  WHERE id IN (-1);

PREPARE null_ret_arr AS
SELECT array_agg(id) FROM edge_table_vertices_pgr  WHERE id IN (-1);

SELECT isnt_empty('edges', 'Should be not empty to tests be meaningful');
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
    '$$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$'
    ,'1::INTEGER',
    '2::INTEGER',
    '3',
    'true'
    ]::TEXT[];
    subs = ARRAY[
    'NULL',
    '(SELECT id::INTEGER FROM edge_table_vertices_pgr  WHERE id IN (-1))',
    '(SELECT id::INTEGER FROM edge_table_vertices_pgr  WHERE id IN (-1))',
    'NULL::INTEGER',
    'NULL'
    ]::TEXT[];

    RETURN query SELECT * FROM no_crash_test('pgr_ksp', params, subs);

    subs = ARRAY[
    'NULL',
    'NULL::INTEGER',
    'NULL::INTEGER',
    'NULL::INTEGER',
    'NULL'
    ]::TEXT[];
    RETURN query SELECT * FROM no_crash_test('pgr_ksp', params, subs);

END
$BODY$
LANGUAGE plpgsql VOLATILE;


SELECT * FROM test_function();

ROLLBACK;
