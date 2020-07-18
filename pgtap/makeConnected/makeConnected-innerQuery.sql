\i setup.sql

SELECT plan(56);

SET client_min_messages TO ERROR;

SELECT has_function('pgr_makeConnected');

SELECT function_returns('pgr_makeConnected',ARRAY['text'],'setof record');
SELECT style_dijkstra('pgr_makeConnected', ')');

SELECT finish();
ROLLBACK;
