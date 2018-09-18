\i setup.sql

SELECT plan(5);

PREPARE edges AS
SELECT id, source, target, cost AS going, reverse_cost AS coming FROM edge_table;

SELECT isnt_empty('edges', 'Should not be empty true to tests be meaningful');


CREATE OR REPLACE FUNCTION test_function()
RETURNS SETOF TEXT AS
$BODY$
DECLARE
params TEXT[];
subs TEXT[];
BEGIN
    params = ARRAY[
        '$$SELECT id, source, target, cost AS going, reverse_cost AS coming FROM edge_table$$'
        ]::TEXT[];
    subs = ARRAY[
        'NULL'
        ]::TEXT[];

    RETURN query SELECT * FROM no_crash_test('pgr_maxCardinalityMatch', params, subs);

END
$BODY$
LANGUAGE plpgsql VOLATILE;


SELECT * FROM test_function();

ROLLBACK;
