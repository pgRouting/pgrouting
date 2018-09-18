\i setup.sql

SELECT plan(7);

PREPARE data AS
SELECT '{{0,1,2,3},{1,0,4,5},{2,4,0,6},{3,5,6,0}}'::float8[];

SELECT isnt_empty('data', 'Should not be empty true to tests be meaningful');

CREATE OR REPLACE FUNCTION test_function()
RETURNS SETOF TEXT AS
$BODY$
DECLARE
params TEXT[];
subs TEXT[];
BEGIN
    params = ARRAY[
    '(SELECT $${{0,1,2,3},{1,0,4,5},{2,4,0,6},{3,5,6,0}}$$::float8[])',
    '1'
    ]::TEXT[];
    subs = ARRAY[
    'NULL',
    'NULL'
    ]::TEXT[];

    RETURN query SELECT * FROM no_crash_test('pgr_tsp', params, subs);
END
$BODY$
LANGUAGE plpgsql VOLATILE;


SELECT * FROM test_function();

ROLLBACK;
