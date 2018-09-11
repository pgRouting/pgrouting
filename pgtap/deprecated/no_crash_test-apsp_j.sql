\i setup.sql

SELECT plan(5);

PREPARE edges AS
SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT  FROM edge_table;

SELECT isnt_empty('edges', 'Should not be empty to tests be meaningful');

CREATE OR REPLACE FUNCTION test_function()
RETURNS SETOF TEXT AS
$BODY$
DECLARE
params TEXT[];
subs TEXT[];
BEGIN
    params = ARRAY['$$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT  FROM edge_table$$']::TEXT[];
    subs = ARRAY[
    'NULL'
    ]::TEXT[];

    RETURN query SELECT * FROM no_crash_test('pgr_apspJohnson', params, subs);
END
$BODY$
LANGUAGE plpgsql VOLATILE;


SELECT * FROM test_function();

ROLLBACK;
