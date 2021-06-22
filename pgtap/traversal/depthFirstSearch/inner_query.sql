\i setup.sql

UPDATE edge_table SET cost = sign(cost), reverse_cost = sign(reverse_cost);
SELECT CASE WHEN min_version('3.2.0') THEN plan (432) ELSE plan(1) END;

CREATE OR REPLACE FUNCTION inner_query()
RETURNS SETOF TEXT AS
$BODY$
BEGIN

IF NOT min_version('3.2.0') THEN
  RETURN QUERY
  SELECT skip(1, 'Function is new on 3.2.0');
  RETURN;
END IF;

-- SINGLE VERTEX
RETURN QUERY
SELECT style_dijkstra('pgr_depthFirstSearch', ', 5)');
-- Single vertex with directed parameter
RETURN QUERY
SELECT style_dijkstra('pgr_depthFirstSearch', ', 5, true)');
-- Single vertex with max_depth parameter
RETURN QUERY
SELECT style_dijkstra('pgr_depthFirstSearch', ', 5, max_depth => 2)');
-- Single vertex with directed and max_depth parameter
RETURN QUERY
SELECT style_dijkstra('pgr_depthFirstSearch', ', 5, true, max_depth => 2)');

-- MULTIPLE VERTICES
RETURN QUERY
SELECT style_dijkstra('pgr_depthFirstSearch', ', ARRAY[3,5])');
-- Multiple vertices with directed parameter
RETURN QUERY
SELECT style_dijkstra('pgr_depthFirstSearch', ', ARRAY[3,5], true)');
-- Multiple vertices with max_depth parameter
RETURN QUERY
SELECT style_dijkstra('pgr_depthFirstSearch', ', ARRAY[3,5], max_depth => 2)');
-- Single vertex with directed and max_depth parameter
RETURN QUERY
SELECT style_dijkstra('pgr_depthFirstSearch', ', ARRAY[3,5], true, max_depth => 2)');

END;
$BODY$
LANGUAGE plpgsql;

SELECT inner_query();

SELECT finish();
ROLLBACK;
