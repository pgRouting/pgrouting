\i setup.sql

SELECT plan(56);

SET client_min_messages TO ERROR;

SELECT has_function('pgr_boyermyrvold');

SELECT function_returns('pgr_boyermyrvold',ARRAY['text'],'setof record');
SELECT style_dijkstra('pgr_boyermyrvold', ')');

SELECT finish();
ROLLBACK;
