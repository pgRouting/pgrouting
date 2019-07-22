\i setup.sql

SELECT plan(416);


SELECT has_function('pgr_bdastarcost', ARRAY['text', 'bigint', 'bigint', 'boolean', 'integer', 'numeric', 'numeric']);
SELECT has_function('pgr_bdastarcost', ARRAY['text', 'bigint', 'anyarray', 'boolean', 'integer', 'numeric', 'numeric']);
SELECT has_function('pgr_bdastarcost', ARRAY['text', 'anyarray', 'bigint', 'boolean', 'integer', 'numeric', 'numeric']);
SELECT has_function('pgr_bdastarcost', ARRAY['text', 'anyarray', 'anyarray', 'boolean', 'integer', 'numeric', 'numeric']);

SELECT function_returns('pgr_bdastarcost', ARRAY['text', 'bigint', 'bigint', 'boolean', 'integer', 'numeric', 'numeric'],
    'setof record');
SELECT function_returns('pgr_bdastarcost', ARRAY['text', 'bigint', 'anyarray', 'boolean', 'integer', 'numeric', 'numeric'],
    'setof record');
SELECT function_returns('pgr_bdastarcost', ARRAY['text', 'anyarray', 'bigint', 'boolean', 'integer', 'numeric', 'numeric'],
    'setof record');
SELECT function_returns('pgr_bdastarcost', ARRAY['text', 'anyarray', 'anyarray', 'boolean', 'integer', 'numeric', 'numeric'],
    'setof record');

SELECT style_astar('pgr_bdastarcost', ', 2, 3, true)');
SELECT style_astar('pgr_bdastarcost', ', 2, ARRAY[3], true)');
SELECT style_astar('pgr_bdastarcost', ', ARRAY[2], 3, true)');
SELECT style_astar('pgr_bdastarcost', ', ARRAY[2], ARRAY[3], true)');

SELECT finish();
ROLLBACK;
