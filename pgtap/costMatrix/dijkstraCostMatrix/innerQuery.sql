\i setup.sql

UPDATE edge_table SET cost = sign(cost), reverse_cost = sign(reverse_cost);
SELECT plan(56);


SELECT has_function('pgr_dijkstracostmatrix',
    ARRAY['text', 'anyarray', 'boolean']);

SELECT function_returns('pgr_dijkstracostmatrix',
    ARRAY['text', 'anyarray', 'boolean'],
    'setof record');


SELECT style_dijkstra('pgr_dijkstracostmatrix', ',ARRAY[2, 3], true)');


SELECT finish();
ROLLBACK;
