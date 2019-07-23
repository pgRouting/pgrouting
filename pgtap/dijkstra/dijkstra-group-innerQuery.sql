\i setup.sql

SELECT plan(224);


SELECT has_function('pgr_dijkstra',
    ARRAY['text', 'bigint', 'bigint', 'boolean']);
SELECT has_function('pgr_dijkstra',
    ARRAY['text', 'bigint', 'anyarray', 'boolean']);
SELECT has_function('pgr_dijkstra',
    ARRAY['text', 'anyarray', 'bigint', 'boolean']);
SELECT has_function('pgr_dijkstra',
    ARRAY['text', 'anyarray', 'anyarray', 'boolean']);

SELECT function_returns('pgr_dijkstra',
    ARRAY['text', 'bigint', 'bigint', 'boolean'],
    'setof record');
SELECT function_returns('pgr_dijkstra',
    ARRAY['text', 'bigint', 'anyarray', 'boolean'],
    'setof record');
SELECT function_returns('pgr_dijkstra',
    ARRAY['text', 'anyarray', 'bigint', 'boolean'],
    'setof record');
SELECT function_returns('pgr_dijkstra',
    ARRAY['text', 'anyarray', 'anyarray', 'boolean'],
    'setof record');


-- ONE TO ONE
SELECT style_dijkstra('pgr_dijkstra', ', 2, 3, true)');
-- ONE TO MANY
SELECT style_dijkstra('pgr_dijkstra', ', 2, ARRAY[3], true)');
-- MANY TO ONE
SELECT style_dijkstra('pgr_dijkstra', ', ARRAY[2], 3, true)');
-- MANY TO MANY
SELECT style_dijkstra('pgr_dijkstra', ', ARRAY[2], ARRAY[3], true)');

SELECT finish();
ROLLBACK;
