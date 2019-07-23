\i setup.sql

SELECT plan(416);


SELECT has_function('pgr_astarcost',
    ARRAY['text', 'bigint', 'bigint', 'boolean', 'integer', 'double precision', 'double precision']);
SELECT has_function('pgr_astarcost',
    ARRAY['text', 'bigint', 'anyarray', 'boolean', 'integer', 'double precision', 'double precision']);
SELECT has_function('pgr_astarcost',
    ARRAY['text', 'anyarray', 'bigint', 'boolean', 'integer', 'double precision', 'double precision']);
SELECT has_function('pgr_astarcost',
    ARRAY['text', 'anyarray', 'anyarray', 'boolean', 'integer', 'double precision', 'double precision']);

SELECT function_returns('pgr_astarcost',
    ARRAY['text', 'bigint', 'bigint', 'boolean', 'integer', 'double precision', 'double precision'],
    'setof record');
SELECT function_returns('pgr_astarcost',
    ARRAY['text', 'bigint', 'anyarray', 'boolean', 'integer', 'double precision', 'double precision'],
    'setof record');
SELECT function_returns('pgr_astarcost',
    ARRAY['text', 'anyarray', 'bigint', 'boolean', 'integer', 'double precision', 'double precision'],
    'setof record');
SELECT function_returns('pgr_astarcost',
    ARRAY['text', 'anyarray', 'anyarray', 'boolean', 'integer', 'double precision', 'double precision'],
    'setof record');

-- ONE TO ONE
SELECT style_astar('pgr_astarcost', ', 2, 3, true)');
-- ONE TO MANY
SELECT style_astar('pgr_astarcost', ', 2, ARRAY[3], true)');
-- MANY TO ONE
SELECT style_astar('pgr_astarcost', ', ARRAY[2], 3, true)');
-- MANY TO MANY
SELECT style_astar('pgr_astarcost', ', ARRAY[2], ARRAY[3], true)');


SELECT finish();
ROLLBACK;
