\i setup.sql

UPDATE edge_table SET cost = sign(cost), reverse_cost = sign(reverse_cost);
SELECT plan(131);

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
params_roadworks TEXT[];
subs_roadworks TEXT[];
BEGIN
    -- one to one
    params = ARRAY[
    '$$SELECT id, source, target, cost, reverse_cost  FROM edge_table$$'
    ,'1::BIGINT',
    '2::BIGINT'
    ]::TEXT[];
    subs = ARRAY[
    'NULL',
    '(SELECT id FROM edge_table_vertices_pgr  WHERE id IN (-1))',
    '(SELECT id FROM edge_table_vertices_pgr  WHERE id IN (-1))'
    ]::TEXT[];

    RETURN query SELECT * FROM no_crash_test('pgr_binaryBreadthFirstSearch', params, subs);

    subs = ARRAY[
    'NULL',
    'NULL::BIGINT',
    'NULL::BIGINT'
    ]::TEXT[];
    RETURN query SELECT * FROM no_crash_test('pgr_binaryBreadthFirstSearch', params, subs);

    -- one to many
    params = ARRAY['$$edges$$','1', 'ARRAY[2,5]::BIGINT[]']::TEXT[];
    subs = ARRAY[
    'NULL',
    '(SELECT id FROM edge_table_vertices_pgr  WHERE id IN (-1))',
    '(SELECT array_agg(id) FROM edge_table_vertices_pgr  WHERE id IN (-1))'
    ]::TEXT[];

    RETURN query SELECT * FROM no_crash_test('pgr_binaryBreadthFirstSearch', params, subs);

    subs = ARRAY[
    'NULL',
    'NULL::BIGINT',
    'NULL::BIGINT[]'
    ]::TEXT[];
    RETURN query SELECT * FROM no_crash_test('pgr_binaryBreadthFirstSearch', params, subs);

    -- many to one
    params = ARRAY['$$edges$$', 'ARRAY[2,5]::BIGINT[]', '1']::TEXT[];
    subs = ARRAY[
    'NULL',
    '(SELECT array_agg(id) FROM edge_table_vertices_pgr  WHERE id IN (-1))',
    '(SELECT id FROM edge_table_vertices_pgr  WHERE id IN (-1))'
    ]::TEXT[];

    RETURN query SELECT * FROM no_crash_test('pgr_binaryBreadthFirstSearch', params, subs);

    subs = ARRAY[
    'NULL',
    'NULL::BIGINT[]',
    'NULL::BIGINT'
    ]::TEXT[];
    RETURN query SELECT * FROM no_crash_test('pgr_binaryBreadthFirstSearch', params, subs);

    -- many to many
    params = ARRAY['$$edges$$','ARRAY[1]::BIGINT[]', 'ARRAY[2,5]::BIGINT[]']::TEXT[];
    subs = ARRAY[
    'NULL',
    '(SELECT array_agg(id) FROM edge_table_vertices_pgr  WHERE id IN (-1))',
    '(SELECT array_agg(id) FROM edge_table_vertices_pgr  WHERE id IN (-1))'
    ]::TEXT[];

    RETURN query SELECT * FROM no_crash_test('pgr_binaryBreadthFirstSearch', params, subs);

    subs = ARRAY[
    'NULL',
    'NULL::BIGINT[]',
    'NULL::BIGINT[]'
    ]::TEXT[];
    RETURN query SELECT * FROM no_crash_test('pgr_binaryBreadthFirstSearch', params, subs);

    -- using roadworks
    -- one to one
    params_roadworks = ARRAY[
    '$$SELECT id, source, target, road_work as cost, reverse_road_work as reverse_cost  FROM roadworks$$'
    ,'1::BIGINT',
    '2::BIGINT'
    ]::TEXT[];
    subs_roadworks = ARRAY[
    'NULL',
    '(SELECT id FROM edge_table_vertices_pgr  WHERE id IN (-1))',
    '(SELECT id FROM edge_table_vertices_pgr  WHERE id IN (-1))'
    ]::TEXT[];

    RETURN query SELECT * FROM no_crash_test('pgr_binaryBreadthFirstSearch', params_roadworks, subs_roadworks);

    subs_roadworks = ARRAY[
    'NULL',
    'NULL::BIGINT',
    'NULL::BIGINT'
    ]::TEXT[];
    RETURN query SELECT * FROM no_crash_test('pgr_binaryBreadthFirstSearch', params_roadworks, subs_roadworks);

    -- one to many
    params_roadworks = ARRAY['$$edges$$','1', 'ARRAY[2,5]::BIGINT[]']::TEXT[];
    subs_roadworks = ARRAY[
    'NULL',
    '(SELECT id FROM edge_table_vertices_pgr  WHERE id IN (-1))',
    '(SELECT array_agg(id) FROM edge_table_vertices_pgr  WHERE id IN (-1))'
    ]::TEXT[];

    RETURN query SELECT * FROM no_crash_test('pgr_binaryBreadthFirstSearch', params_roadworks, subs_roadworks);

    subs_roadworks = ARRAY[
    'NULL',
    'NULL::BIGINT',
    'NULL::BIGINT[]'
    ]::TEXT[];
    RETURN query SELECT * FROM no_crash_test('pgr_binaryBreadthFirstSearch', params_roadworks, subs_roadworks);

    -- many to one
    params_roadworks = ARRAY['$$edges$$', 'ARRAY[2,5]::BIGINT[]', '1']::TEXT[];
    subs_roadworks = ARRAY[
    'NULL',
    '(SELECT array_agg(id) FROM edge_table_vertices_pgr  WHERE id IN (-1))',
    '(SELECT id FROM edge_table_vertices_pgr  WHERE id IN (-1))'
    ]::TEXT[];

    RETURN query SELECT * FROM no_crash_test('pgr_binaryBreadthFirstSearch', params_roadworks, subs_roadworks);

    subs_roadworks = ARRAY[
    'NULL',
    'NULL::BIGINT[]',
    'NULL::BIGINT'
    ]::TEXT[];
    RETURN query SELECT * FROM no_crash_test('pgr_binaryBreadthFirstSearch', params_roadworks, subs_roadworks);

    -- many to many
    params_roadworks = ARRAY['$$edges$$','ARRAY[1]::BIGINT[]', 'ARRAY[2,5]::BIGINT[]']::TEXT[];
    subs_roadworks = ARRAY[
    'NULL',
    '(SELECT array_agg(id) FROM edge_table_vertices_pgr  WHERE id IN (-1))',
    '(SELECT array_agg(id) FROM edge_table_vertices_pgr  WHERE id IN (-1))'
    ]::TEXT[];

    RETURN query SELECT * FROM no_crash_test('pgr_binaryBreadthFirstSearch', params_roadworks, subs_roadworks);

    subs_roadworks = ARRAY[
    'NULL',
    'NULL::BIGINT[]',
    'NULL::BIGINT[]'
    ]::TEXT[];
    RETURN query SELECT * FROM no_crash_test('pgr_binaryBreadthFirstSearch', params_roadworks, subs_roadworks);

END
$BODY$
LANGUAGE plpgsql VOLATILE;


SELECT * FROM test_function();

ROLLBACK;
