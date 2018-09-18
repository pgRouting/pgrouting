\i setup.sql

SELECT plan(26);

PREPARE edges AS
SELECT id, source, target, cost, reverse_cost FROM edge_table;

PREPARE null_ret AS
SELECT id FROM edge_table_vertices_pgr  WHERE id IN (-1);

SELECT isnt_empty('edges', 'Should not be empty to tests be meaningful');
SELECT is_empty('null_ret', 'Should be empty to tests be meaningful');


CREATE OR REPLACE FUNCTION test_function()
RETURNS SETOF TEXT AS
$BODY$
DECLARE
params TEXT[];
subs TEXT[];
BEGIN
    params = ARRAY['$$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT  FROM edge_table$$',
    '1',
    'ARRAY[2]', 'true', 'true']::TEXT[];
    subs = ARRAY[
    'NULL',
    '(SELECT id::INTEGER FROM edge_table_vertices_pgr  WHERE id IN (-1))',
    '(SELECT array_agg(id)::INTEGER[] FROM edge_table_vertices_pgr  WHERE id IN (-1))',
    'NULL',
    'NULL'
    ]::TEXT[];

    RETURN query SELECT * FROM no_crash_test('pgr_kdijkstraCost', params, subs);

    subs = ARRAY[
    'NULL',
    'NULL',
    'NULL::INTEGER[]',
    'NULL',
    'NULL'
    ]::TEXT[];
    RETURN query SELECT * FROM no_crash_test('pgr_kdijkstraCost', params, subs);

END
$BODY$
LANGUAGE plpgsql VOLATILE;


SELECT * FROM test_function();

ROLLBACK;
