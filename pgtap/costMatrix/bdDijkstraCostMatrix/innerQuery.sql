\i setup.sql

UPDATE edge_table SET cost = sign(cost), reverse_cost = sign(reverse_cost);
SELECT plan(56);


SELECT has_function('pgr_bddijkstracostmatrix',
    ARRAY['text', 'anyarray', 'boolean']);

SELECT function_returns('pgr_bddijkstracostmatrix',
    ARRAY['text', 'anyarray', 'boolean'],
    'setof record');


SELECT style_dijkstra('pgr_bddijkstracostmatrix', ',ARRAY[2, 3], true)');


SELECT finish();
ROLLBACK;
