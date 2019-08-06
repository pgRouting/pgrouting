\i setup.sql

SELECT plan(3);
-- flags
-- error

SELECT throws_ok(
    'SELECT * FROM pgr_topologicalsort(
        ''SELECT id, source, target, cost, reverse_cost FROM edge_table id < 2'',
        3
    )','42883','function pgr_topologicalsort(unknown, integer) does not exist',
    '6: Documentation says it does not work with 1 flags');


SELECT lives_ok(
    'SELECT * FROM pgr_topologicalsort(
        ''SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 2 ''
    )',
    '4: Documentation says works with no flags');


CREATE OR REPLACE FUNCTION test_function()
RETURNS SETOF TEXT AS
$BODY$
DECLARE
params TEXT[];
subs TEXT[];
BEGIN
    params = ARRAY[
    '$$SELECT id, source, target, cost, reverse_cost  FROM edge_table WHERE id = 1 $$'
    ]::TEXT[];
    subs = ARRAY[
    'NULL',
    'NULL::BIGINT[]'

    ]::TEXT[];

    RETURN query SELECT * FROM no_crash_test('pgr_transitiveclosure', params, subs);

END
$BODY$
LANGUAGE plpgsql VOLATILE;

SELECT * FROM test_function();

ROLLBACK;
