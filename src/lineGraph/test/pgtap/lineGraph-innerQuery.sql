\i setup.sql

SELECT plan(137);
SET client_min_messages TO ERROR;


SELECT has_function('pgr_linegraph',
    ARRAY['text', 'bigint', 'bigint', 'boolean','boolean']);

SELECT function_returns('pgr_linegraph',
    ARRAY['text', 'bigint', 'bigint', 'boolean','boolean'],
    'setof record');

SELECT style_dijkstra('pgr_linegraph', ', 2, 3)');
SELECT style_dijkstra('pgr_linegraph', ', 2, 3, true)');
SELECT style_dijkstra('pgr_linegraph', ', 2, 3, false)');



SELECT finish();
ROLLBACK;
