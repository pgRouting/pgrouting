\i setup.sql

SELECT plan(56);

SET client_min_messages TO ERROR;

SELECT has_function('pgr_boyermyrvold_1');

SELECT function_returns('pgr_boyermyrvold_1',ARRAY['text'],'setof record');
SELECT style_dijkstra('pgr_boyermyrvold_1', ')');

SELECT finish();
ROLLBACK;
