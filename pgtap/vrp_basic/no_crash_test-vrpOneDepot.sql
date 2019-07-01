\i setup.sql

SELECT plan(13);

PREPARE orders AS
SELECT * FROM solomon_100_RC_101;
PREPARE vehicles AS
SELECT * FROM vrp_vehicles;
PREPARE distance AS
SELECT * FROM vrp_distance;

SELECT isnt_empty('orders', 'Should be not empty to tests be meaningful');
SELECT isnt_empty('vehicles', 'Should be not empty to tests be meaningful');
SELECT isnt_empty('distance', 'Should be not empty to tests be meaningful');


CREATE OR REPLACE FUNCTION test_function()
RETURNS SETOF TEXT AS
$BODY$
DECLARE
params TEXT[];
subs TEXT[];
BEGIN
    -- one to one
    params = ARRAY[
    '$$SELECT * FROM solomon_100_RC_101 ORDER BY id$$',
    '$$SELECT * FROM vrp_vehicles$$',
    '$$SELECT * FROM vrp_distance$$',
    '1'
    ]::TEXT[];
    subs = ARRAY[
    'NULL',
    'NULL',
    'NULL',
    'NULL'
    ]::TEXT[];
    RETURN query SELECT * FROM no_crash_test('pgr_vrpOneDepot', params, subs);
END
$BODY$
LANGUAGE plpgsql VOLATILE;


SELECT * FROM test_function();

ROLLBACK;
