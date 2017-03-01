\i setup.sql

SELECT plan(348);


SELECT has_function('pgr_bdastar', ARRAY['text', 'bigint', 'bigint', 'boolean', 'integer', 'double precision', 'double precision']);
SELECT has_function('pgr_bdastar', ARRAY['text', 'bigint', 'anyarray', 'boolean', 'integer', 'double precision', 'double precision']);
SELECT has_function('pgr_bdastar', ARRAY['text', 'anyarray', 'bigint', 'boolean', 'integer', 'double precision', 'double precision']);
SELECT has_function('pgr_bdastar', ARRAY['text', 'anyarray', 'anyarray', 'boolean', 'integer', 'double precision', 'double precision']);

SELECT function_returns('pgr_bdastar', ARRAY['text', 'bigint', 'bigint', 'boolean', 'integer', 'double precision', 'double precision'],
    'setof record');
SELECT function_returns('pgr_bdastar', ARRAY['text', 'bigint', 'anyarray', 'boolean', 'integer', 'double precision', 'double precision'],
    'setof record');
SELECT function_returns('pgr_bdastar', ARRAY['text', 'anyarray', 'bigint', 'boolean', 'integer', 'double precision', 'double precision'],
    'setof record');
SELECT function_returns('pgr_bdastar', ARRAY['text', 'anyarray', 'anyarray', 'boolean', 'integer', 'double precision', 'double precision'],
    'setof record');

SELECT style_astar('pgr_bdastar', ', 2, 3, true)');
SELECT style_astar('pgr_bdastar', ', 2, ARRAY[3], true)');
SELECT style_astar('pgr_bdastar', ', ARRAY[2], 3, true)');
SELECT style_astar('pgr_bdastar', ', ARRAY[2], ARRAY[3], true)');

SELECT finish();
ROLLBACK;
