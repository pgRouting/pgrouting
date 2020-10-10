\i setup.sql

UPDATE edge_table SET cost = sign(cost), reverse_cost = sign(reverse_cost);
SELECT plan(56);


SELECT has_function('pgr_stoerwagner');

SELECT function_returns('pgr_stoerwagner', ARRAY['text'], 'setof record');

SELECT style_dijkstra('pgr_stoerwagner', ')');

SELECT finish();
ROLLBACK;
