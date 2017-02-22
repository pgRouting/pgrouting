\i setup.sql

SELECT plan(47);

SELECT has_function('pgr_bddijkstra',
    ARRAY['text', 'bigint', 'bigint', 'boolean']);

SELECT function_returns('pgr_bddijkstra',
    ARRAY['text', 'bigint', 'bigint', 'boolean'],
    'setof record');



SELECT style_dijkstra('pgr_bddijkstra', ', 2,3, true)');
/*
SELECT style_dijkstra('pgr_bddijkstra', ', 2, ARRAY[3], true)');
SELECT style_dijkstra('pgr_bddijkstra', ',  2, ARRAY[3], true)');
SELECT style_dijkstra('pgr_bddijkstra', ',  ARRAY[2], ARRAY[3], true)');
*/


SELECT finish();
ROLLBACK;
