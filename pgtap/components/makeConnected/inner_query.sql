\i setup.sql

UPDATE edge_table SET cost = sign(cost), reverse_cost = sign(reverse_cost);
SELECT plan(56);

SET client_min_messages TO ERROR;

SELECT has_function('pgr_makeconnected');

SELECT function_returns('pgr_makeconnected',ARRAY['text'],'setof record');
SELECT style_dijkstra('pgr_makeconnected', ')');

SELECT finish();
ROLLBACK;
