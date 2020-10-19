\i setup.sql

SELECT plan(126);
UPDATE edge_table SET cost = sign(cost), reverse_cost = sign(reverse_cost);

PREPARE edges AS
SELECT id, source, target, cost, reverse_cost  FROM edge_table;

PREPARE combs AS
SELECT source, target  FROM combinations_table;
SELECT isnt_empty('edges', 'Should be not empty to tests be meaningful');
SELECT isnt_empty('combs', 'Should be not empty to tests be meaningful');

PREPARE null_id AS
SELECT id FROM edge_table_vertices_pgr  WHERE id IN (-1);
SELECT is_empty('null_id', 'Should be empty to tests be meaningful');

PREPARE null_id_arr AS
SELECT array_agg(id) FROM edge_table_vertices_pgr  WHERE id IN (-1);
SELECT set_eq('null_id_arr', 'SELECT NULL::BIGINT[]', 'Should be empty to tests be meaningful');

PREPARE null_edges AS
SELECT id, source, target, cost, reverse_cost  FROM edge_table WHERE id IN (-1);
SELECT is_empty('null_edges', 'Should be empty to tests be meaningful');

PREPARE null_comb AS
SELECT source, target FROM combinations_table  WHERE source IN (-1);
SELECT is_empty('null_comb', 'Should be empty to tests be meaningful');


CREATE OR REPLACE FUNCTION test_function()
RETURNS SETOF TEXT AS
$BODY$
DECLARE
params1 TEXT[];
params2 TEXT[];
subs1 TEXT[];
subs2 TEXT[];
BEGIN
    -- one to many
    params1 = ARRAY['$$edges$$','1', 'ARRAY[2,5]::BIGINT[]']::TEXT[];
    params2 = ARRAY['$$SELECT id, source, target, cost, reverse_cost  FROM edge_table$$',
            '(SELECT 1)',
            '(SELECT ARRAY[2,5]::BIGINT[])']::TEXT[];
    subs1 = ARRAY[
            '$$null_edges$$',
            '(SELECT id FROM edge_table_vertices_pgr  WHERE id IN (-1))',
            '(SELECT array_agg(id) FROM edge_table_vertices_pgr  WHERE id IN (-1))'
            ]::TEXT[];
    subs2 = ARRAY[
            'NULL',
            'NULL::BIGINT',
            'NULL::BIGINT[]'
            ]::TEXT[];

    RETURN query SELECT * FROM no_crash_test('pgr_dijkstranear', params1, subs1);
    RETURN query SELECT * FROM no_crash_test('pgr_dijkstraNear', params1, subs2);
    RETURN query SELECT * FROM no_crash_test('pgr_dijkstranear', params2, subs1);
    RETURN query SELECT * FROM no_crash_test('pgr_dijkstraNear', params2, subs2);


    -- many to one
    params1 = ARRAY['$$edges$$', 'ARRAY[2,5]::BIGINT[]', '1']::TEXT[];
    params2 = ARRAY['$$SELECT id, source, target, cost, reverse_cost  FROM edge_table$$',
            '(SELECT ARRAY[2,5]::BIGINT[])',
            '(SELECT 1)']::TEXT[];
    subs1 = ARRAY[
            '$$null_edges$$',
            '(SELECT array_agg(id) FROM edge_table_vertices_pgr  WHERE id IN (-1))',
            '(SELECT id FROM edge_table_vertices_pgr  WHERE id IN (-1))'
            ]::TEXT[];
    subs2 = ARRAY[
            'NULL',
            'NULL::BIGINT[]',
            'NULL::BIGINT'
            ]::TEXT[];

    RETURN query SELECT * FROM no_crash_test('pgr_dijkstranear', params1, subs1);
    RETURN query SELECT * FROM no_crash_test('pgr_dijkstraNear', params1, subs2);
    RETURN query SELECT * FROM no_crash_test('pgr_dijkstranear', params2, subs1);
    RETURN query SELECT * FROM no_crash_test('pgr_dijkstraNear', params2, subs2);

    -- many to many
    params1 = ARRAY['$$edges$$','ARRAY[1]::BIGINT[]', 'ARRAY[2,5]::BIGINT[]']::TEXT[];
    params2 = ARRAY['$$SELECT id, source, target, cost, reverse_cost  FROM edge_table$$',
            '(SELECT ARRAY[2,5]::BIGINT[])',
            '(SELECT ARRAY[1]::BIGINT[])']::TEXT[];
    subs1 = ARRAY[
            '$$null_edges$$',
            '(SELECT array_agg(id) FROM edge_table_vertices_pgr  WHERE id IN (-1))',
            '(SELECT array_agg(id) FROM edge_table_vertices_pgr  WHERE id IN (-1))'
            ]::TEXT[];
    subs2 = ARRAY[
            'NULL',
            'NULL::BIGINT[]',
            'NULL::BIGINT[]'
            ]::TEXT[];

    RETURN query SELECT * FROM no_crash_test('pgr_dijkstranear', params1, subs1);
    RETURN query SELECT * FROM no_crash_test('pgr_dijkstraNear', params1, subs2);
    RETURN query SELECT * FROM no_crash_test('pgr_dijkstranear', params2, subs1);
    RETURN query SELECT * FROM no_crash_test('pgr_dijkstraNear', params2, subs2);

    -- combinations
    params1 = ARRAY['$$edges$$','$$combs$$']::TEXT[];
    params2 = ARRAY['$$SELECT id, source, target, cost, reverse_cost  FROM edge_table$$',
            '$$SELECT source, target FROM edge_table$$']::TEXT[];
    subs1 = ARRAY[
            '$$null_edges$$',
            '$$null_comb$$'
            ]::TEXT;
    subs2 = ARRAY[
            'NULL',
            'NULL'
            ]::TEXT[];

    RETURN query SELECT * FROM no_crash_test('pgr_dijkstranear', params1, subs1);
    RETURN query SELECT * FROM no_crash_test('pgr_dijkstraNear', params1, subs2);
    RETURN query SELECT * FROM no_crash_test('pgr_dijkstranear', params2, subs1);
    RETURN query SELECT * FROM no_crash_test('pgr_dijkstraNear', params2, subs2);
END
$BODY$
LANGUAGE plpgsql VOLATILE;


SELECT * FROM test_function();

ROLLBACK;
