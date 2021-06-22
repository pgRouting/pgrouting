\i setup.sql

UPDATE edge_table SET cost = sign(cost), reverse_cost = sign(reverse_cost);

SELECT CASE
WHEN NOT min_lib_version('3.1.1') THEN plan(3)
ELSE plan(56) END;

CREATE OR REPLACE FUNCTION inner_query()
RETURNS SETOF TEXT AS
$BODY$
BEGIN

RETURN QUERY
SELECT has_function('pgr_chinesepostman',
    ARRAY['text']);

RETURN QUERY
SELECT function_returns('pgr_chinesepostman',
    ARRAY['text'],
    'setof record');

DELETE FROM edge_table WHERE id > 16;

IF NOT min_lib_version('3.1.1') THEN
  RETURN QUERY
  SELECT skip(1, 'Server crash fixed on 3.1.1');
  RETURN;
END IF;

RETURN QUERY
SELECT style_dijkstra('pgr_chinesepostman', ')');


END;
$BODY$
LANGUAGE plpgsql;

SELECT inner_query();



SELECT finish();
ROLLBACK;
