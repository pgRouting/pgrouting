\i setup.sql

UPDATE edge_table SET cost = sign(cost), reverse_cost = sign(reverse_cost);
SELECT plan(226);


SELECT has_function('pgr_dijkstracost',
    ARRAY['text', 'bigint', 'bigint', 'boolean']);
SELECT has_function('pgr_dijkstracost',
    ARRAY['text', 'bigint', 'anyarray', 'boolean']);
SELECT has_function('pgr_dijkstracost',
    ARRAY['text', 'anyarray', 'bigint', 'boolean']);
SELECT has_function('pgr_dijkstracost',
    ARRAY['text', 'anyarray', 'anyarray', 'boolean']);
SELECT has_function('pgr_dijkstracost',
    ARRAY['text', 'text', 'boolean']);

SELECT function_returns('pgr_dijkstracost',
    ARRAY['text', 'bigint', 'bigint', 'boolean'],
    'setof record');
SELECT function_returns('pgr_dijkstracost',
    ARRAY['text', 'bigint', 'anyarray', 'boolean'],
    'setof record');
SELECT function_returns('pgr_dijkstracost',
    ARRAY['text', 'anyarray', 'bigint', 'boolean'],
    'setof record');
SELECT function_returns('pgr_dijkstracost',
    ARRAY['text', 'anyarray', 'anyarray', 'boolean'],
    'setof record');
SELECT function_returns('pgr_dijkstracost',
    ARRAY['text', 'text','boolean'],
    'setof record');


-- ONE TO ONE
SELECT style_dijkstra('pgr_dijkstracost', ', 2, 3, true)');
-- ONE TO MANY
SELECT style_dijkstra('pgr_dijkstracost', ', 2, ARRAY[3], true)');
-- MANY TO ONE
SELECT style_dijkstra('pgr_dijkstracost', ', ARRAY[2], 3, true)');
-- MANY TO MANY
SELECT style_dijkstra('pgr_dijkstracost', ', ARRAY[2], ARRAY[3], true)');


SELECT finish();
ROLLBACK;
