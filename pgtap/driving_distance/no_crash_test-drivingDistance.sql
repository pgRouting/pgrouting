\i setup.sql

SELECT plan(34);

PREPARE edges AS
SELECT id, source, target, cost, reverse_cost  FROM edge_table;

PREPARE null_ret AS
SELECT array_agg(id) FROM edge_table_vertices_pgr  WHERE id IN (-1);

SELECT isnt_empty('edges', 'Should be not empty to tests be meaningful');
SELECT set_eq('null_ret', 'SELECT NULL::BIGINT[]', 'Should be empty to tests be meaningful');


CREATE OR REPLACE FUNCTION test_function()
RETURNS SETOF TEXT AS
$BODY$
DECLARE
params TEXT[];
subs TEXT[];
BEGIN
    -- 1 to distance
    params = ARRAY['$$SELECT id, source, target, cost, reverse_cost  FROM edge_table$$','1', '1.3::FLOAT']::TEXT[];
    subs = ARRAY[
    'NULL',
    '(SELECT id FROM edge_table_vertices_pgr  WHERE id IN (-1))',
    'NULL'
    ]::TEXT[];
    RETURN query SELECT * FROM no_crash_test('pgr_drivingDistance', params, subs);

    subs = ARRAY[
    'NULL',
    'NULL::BIGINT',
    'NULL'
    ]::TEXT[];
    RETURN query SELECT * FROM no_crash_test('pgr_drivingDistance', params, subs);

    -- many to distance
    params = ARRAY['$$SELECT id, source, target, cost, reverse_cost  FROM edge_table$$',
    'ARRAY[1]', '1.3::FLOAT']::TEXT[];
    subs = ARRAY[
    'NULL',
    '(SELECT array_agg(id) FROM edge_table_vertices_pgr  WHERE id IN (-1))',
    'NULL'
    ]::TEXT[];
    RETURN query SELECT * FROM no_crash_test('pgr_drivingDistance', params, subs);

    subs = ARRAY[
    'NULL',
    'NULL::BIGINT[]',
    'NULL'
    ]::TEXT[];
    RETURN query SELECT * FROM no_crash_test('pgr_drivingDistance', params, subs);

END
$BODY$
LANGUAGE plpgsql VOLATILE;


SELECT * FROM test_function();

ROLLBACK;
