\i setup.sql

SELECT plan(206);


SELECT has_function('pgr_bdastarcostmatrix', ARRAY['text', 'anyarray', 'boolean', 'integer', 'numeric', 'numeric']);

SELECT function_returns('pgr_bdastarcostmatrix', ARRAY['text', 'anyarray', 'boolean', 'integer', 'numeric', 'numeric'],
    'setof record');

SELECT style_astar('pgr_bdastarcostmatrix', ', ARRAY[2, 3], true)');
SELECT style_astar('pgr_bdastarcostmatrix', ', ARRAY[2, 3], false)');

SELECT finish();
ROLLBACK;
