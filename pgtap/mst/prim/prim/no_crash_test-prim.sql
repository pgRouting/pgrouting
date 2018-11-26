\i setup.sql

SELECT plan(9);

PREPARE edges AS
SELECT id, source, target, cost, reverse_cost  FROM edge_table;

SELECT isnt_empty('edges', 'Should be not empty to tests be meaningful');


CREATE OR REPLACE FUNCTION test_function()
RETURNS SETOF TEXT AS
$BODY$
DECLARE
params TEXT[];
subs TEXT[];
BEGIN
    -- prim with no root vertex
    params = ARRAY[
    '$$SELECT id, source, target, cost, reverse_cost  FROM edge_table$$'
    ]::TEXT[];

    subs = ARRAY[
    'NULL'
    ]::TEXT[];

    RETURN query SELECT * FROM no_crash_test('pgr_prim', params, subs);

    params[1] := '$$edges$$';
    RETURN query SELECT * FROM no_crash_test('pgr_prim', params, subs);

END
$BODY$
LANGUAGE plpgsql VOLATILE;


SELECT * FROM test_function();

ROLLBACK;
