\i setup.sql

SELECT plan(47);


SELECT has_function('pgr_topologicalSort');

SELECT function_returns('pgr_topologicalSort', ARRAY['text'], 'setof record');

SELECT style_dijkstra('pgr_topologicalSort', ')');

SELECT finish();
ROLLBACK;
