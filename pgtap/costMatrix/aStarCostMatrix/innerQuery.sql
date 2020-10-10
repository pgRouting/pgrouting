\i setup.sql

UPDATE edge_table SET cost = sign(cost), reverse_cost = sign(reverse_cost);
SELECT plan(104);


SELECT has_function('pgr_astarcostmatrix',
    ARRAY['text', 'anyarray', 'boolean', 'integer', 'double precision', 'double precision']);

SELECT function_returns('pgr_astarcostmatrix',
    ARRAY['text', 'anyarray', 'boolean', 'integer', 'double precision', 'double precision'],
    'setof record');


SELECT style_astar('pgr_astarcostmatrix', ',ARRAY[2, 3], true)');



SELECT finish();
ROLLBACK;
