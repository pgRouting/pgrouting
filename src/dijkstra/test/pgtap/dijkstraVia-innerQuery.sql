\i setup.sql

SELECT plan(47);


SELECT has_function('pgr_dijkstravia',
    ARRAY['text', 'anyarray', 'boolean', 'boolean', 'boolean']);

SELECT function_returns('pgr_dijkstravia',
    ARRAY['text', 'anyarray', 'boolean', 'boolean', 'boolean'],
    'setof record');

SELECT style_dijkstra('pgr_dijkstravia', ', ARRAY[2, 3], true, true)');


SELECT finish();
ROLLBACK;
