\i setup.sql

SELECT plan(56);


SELECT has_function('pgr_stoerwagner');

SELECT function_returns('pgr_stoerwagner', ARRAY['text'], 'setof record');

SELECT style_dijkstra('pgr_stoerwagner', ')');

SELECT finish();
ROLLBACK;
