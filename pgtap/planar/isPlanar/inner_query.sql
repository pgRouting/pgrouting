\i setup.sql

UPDATE edge_table SET cost = sign(cost), reverse_cost = sign(reverse_cost);
SELECT plan(56);

SET client_min_messages TO ERROR;

SELECT has_function('pgr_isplanar');

SELECT function_returns('pgr_isplanar',ARRAY['text'],'boolean');
SELECT style_dijkstra('pgr_isplanar', ')');

SELECT finish();
ROLLBACK;
