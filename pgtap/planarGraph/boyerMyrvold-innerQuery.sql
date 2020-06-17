\i setup.sql

SELECT plan(56);

SET client_min_messages TO ERROR;

SELECT has_function('pgr_boyerMyrvold');

SELECT function_returns('pgr_boyerMyrvold',ARRAY['text'],'setof record');
SELECT style_dijkstra('pgr_boyerMyrvold', ')');

SELECT finish();
ROLLBACK;
