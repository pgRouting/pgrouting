\i setup.sql

UPDATE edge_table SET cost = sign(cost), reverse_cost = sign(reverse_cost);
SELECT CASE WHEN NOT min_version('3.2.0') THEN plan(1) ELSE plan(54) END;


CREATE OR REPLACE FUNCTION inner_query()
RETURNS SETOF TEXT AS
$BODY$
BEGIN

IF NOT min_version('3.2.0') THEN
  RETURN QUERY
  SELECT skip(1, 'Function is new on 3.2.0');
  RETURN;
END IF;

RETURN QUERY
SELECT style_dijkstra('pgr_lengauerTarjanDominatorTree', ',1)');

END;
$BODY$
LANGUAGE plpgsql;

SELECT inner_query();

SELECT finish();
ROLLBACK;
