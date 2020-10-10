\i setup.sql

UPDATE edge_table SET cost = sign(cost), reverse_cost = sign(reverse_cost);
SELECT plan(56);


SELECT has_function('pgr_withpointscostmatrix',
    ARRAY['text', 'text', 'anyarray', 'boolean', 'character']);

SELECT function_returns('pgr_withpointscostmatrix',
    ARRAY['text', 'text', 'anyarray', 'boolean', 'character'],
    'setof record');


SELECT style_withpoints('pgr_withpointscostmatrix', ', ARRAY[2, 3], true)');



SELECT finish();
ROLLBACK;
