\i setup.sql

UPDATE edge_table SET cost = sign(cost), reverse_cost = sign(reverse_cost);
SELECT CASE WHEN min_version('3.2.0') THEN plan (56) ELSE plan(1) END;

CREATE OR REPLACE FUNCTION inner_query()
RETURNS SETOF TEXT AS
$BODY$
BEGIN

IF NOT min_version('3.2.0') THEN
  RETURN QUERY
  SELECT skip(1, 'Function is new on 3.2.0');
  RETURN;
END IF;

SET client_min_messages TO ERROR;

RETURN QUERY
SELECT has_function('pgr_isplanar');

RETURN QUERY
SELECT function_returns('pgr_isplanar',ARRAY['text'],'boolean');

RETURN QUERY
SELECT style_dijkstra('pgr_isplanar', ')');

END;
$BODY$
LANGUAGE plpgsql;

SELECT inner_query();

SELECT finish();
ROLLBACK;
