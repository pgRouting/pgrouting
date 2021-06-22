\i setup.sql

UPDATE edge_table SET cost = sign(cost), reverse_cost = sign(reverse_cost);
SELECT CASE WHEN NOT min_version('3.2.0') THEN plan(1) ELSE plan(5) END;

PREPARE edges AS
SELECT id, source, target, cost, reverse_cost  FROM edge_table;


CREATE OR REPLACE FUNCTION test_function()
RETURNS SETOF TEXT AS
$BODY$
DECLARE
params TEXT[];
subs TEXT[];
BEGIN
  IF NOT min_version('3.2.0') THEN
    RETURN QUERY
    SELECT skip(1, 'Function is new on 3.2.0');
    RETURN;
  END IF;

 RETURN QUERY
 SELECT isnt_empty('edges', 'Should not be empty true to tests be meaningful');

    params = ARRAY['$$SELECT id, source, target, cost, reverse_cost  FROM edge_table$$']::TEXT[];
    subs = ARRAY[
    'NULL'
    ]::TEXT[];

    RETURN query SELECT * FROM no_crash_test('pgr_makeConnected', params, subs);
END
$BODY$
LANGUAGE plpgsql VOLATILE;


SELECT * FROM test_function();

ROLLBACK;
