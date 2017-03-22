\i setup.sql

SELECT plan(94);


SELECT has_function('pgr_withpointsdd',
    ARRAY['text', 'text', 'bigint', 'double precision', 'boolean', 'character', 'boolean']);
SELECT has_function('pgr_withpointsdd',
    ARRAY['text', 'text', 'anyarray', 'double precision', 'boolean', 'character', 'boolean', 'boolean']);

SELECT function_returns('pgr_withpointsdd',
    ARRAY['text', 'text', 'bigint', 'double precision', 'boolean', 'character', 'boolean'],
    'setof record');
SELECT function_returns('pgr_withpointsdd',
    ARRAY['text', 'text', 'anyarray', 'double precision', 'boolean', 'character', 'boolean', 'boolean'],
    'setof record');


-- ONE SOURCE
SELECT style_withpoints('pgr_withpointsdd', ', 2, 3, true)');


-- MANY SOURCES
SELECT style_withpoints('pgr_withpointsdd', ', ARRAY[2], 3, true)');



SELECT finish();
ROLLBACK;
