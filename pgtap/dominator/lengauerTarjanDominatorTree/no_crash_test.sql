\i setup.sql

UPDATE edge_table SET cost = sign(cost), reverse_cost = sign(reverse_cost);
SELECT CASE WHEN NOT min_version('3.2.0') THEN plan(1) ELSE plan(7) END;

PREPARE edges AS
SELECT id, source, target, cost, reverse_cost  FROM edge_table;


CREATE OR REPLACE FUNCTION no_crash()
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
SELECT isnt_empty('edges', 'Should be not empty to tests be meaningful');

params = ARRAY[
  '$$SELECT id, source, target, cost, reverse_cost  FROM edge_table$$',
  '1::BIGINT'
  ]::TEXT[];
subs = ARRAY[
  'NULL',
  '(SELECT id FROM edge_table_vertices_pgr  WHERE id IN (-1))'
  ]::TEXT[];

RETURN query SELECT * FROM no_crash_test('pgr_lengauerTarjanDominatorTree', params, subs);
END
$BODY$
LANGUAGE plpgsql VOLATILE;


SELECT * FROM no_crash();



ROLLBACK;
