\i setup.sql

SELECT plan(9);

PREPARE edges AS
SELECT * FROM customer ORDER BY id;


SELECT isnt_empty('edges', 'Should be not empty to tests be meaningful');


CREATE OR REPLACE FUNCTION test_function()
RETURNS SETOF TEXT AS
$BODY$
DECLARE
params TEXT[];
subs TEXT[];
BEGIN
    -- one to one
    params = ARRAY[
    '$$SELECT * FROM customer ORDER BY id$$',
    '25', '200']::TEXT[];
    subs = ARRAY[
    'NULL',
    'NULL',
    'NULL'
    ]::TEXT[];
    RETURN query SELECT * FROM no_crash_test('pgr_gsoc_vrppdtw', params, subs);
END
$BODY$
LANGUAGE plpgsql VOLATILE;


SELECT * FROM test_function();

ROLLBACK;
