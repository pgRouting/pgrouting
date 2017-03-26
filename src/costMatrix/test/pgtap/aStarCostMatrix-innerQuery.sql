\i setup.sql

SELECT plan(87);


SELECT has_function('pgr_astarcostmatrix',
    ARRAY['text', 'anyarray', 'boolean', 'integer', 'numeric', 'numeric']);

SELECT function_returns('pgr_astarcostmatrix',
    ARRAY['text', 'anyarray', 'boolean', 'integer', 'numeric', 'numeric'],
    'setof record');


SELECT style_astar('pgr_astarcostmatrix', ',ARRAY[2, 3], true)');



SELECT finish();
ROLLBACK;
