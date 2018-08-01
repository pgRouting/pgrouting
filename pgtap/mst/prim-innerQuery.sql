\i setup.sql

SELECT plan(93);


SELECT has_function('pgr_prim');

SELECT has_function('pgr_prim', ARRAY['text', 'bigint']);

SELECT function_returns('pgr_prim', ARRAY['text', 'bigint'], 'setof record');

SELECT style_dijkstra('pgr_prim', ', 4)');
SELECT style_dijkstra('pgr_prim', ')');

SELECT finish();
ROLLBACK;
