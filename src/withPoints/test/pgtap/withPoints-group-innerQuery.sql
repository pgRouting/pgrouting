\i setup.sql

SELECT plan(548);
SET client_min_messages TO ERROR;


SELECT has_function('pgr_withpoints',
    ARRAY['text', 'text', 'bigint', 'bigint', 'boolean','character', 'boolean']);
SELECT has_function('pgr_withpoints',
    ARRAY['text', 'text', 'bigint', 'anyarray', 'boolean','character', 'boolean']);
SELECT has_function('pgr_withpoints',
    ARRAY['text', 'text', 'anyarray', 'bigint', 'boolean','character', 'boolean']);
SELECT has_function('pgr_withpoints',
    ARRAY['text', 'text', 'anyarray', 'anyarray', 'boolean','character', 'boolean']);

SELECT function_returns('pgr_withpoints',
    ARRAY['text', 'text', 'bigint', 'bigint', 'boolean','character', 'boolean'],
    'setof record');
SELECT function_returns('pgr_withpoints',
    ARRAY['text', 'text', 'bigint', 'anyarray', 'boolean','character', 'boolean'],
    'setof record');
SELECT function_returns('pgr_withpoints',
    ARRAY['text', 'text', 'anyarray', 'bigint', 'boolean','character', 'boolean'],
    'setof record');
SELECT function_returns('pgr_withpoints',
    ARRAY['text', 'text', 'anyarray', 'anyarray', 'boolean','character', 'boolean'],
    'setof record');


-- DIRECTED
SELECT style_withpoints('pgr_withpoints', ', 2, 3)');
SELECT style_withpoints('pgr_withpoints', ', 2, ARRAY[3])');
SELECT style_withpoints('pgr_withpoints', ', ARRAY[2], 3)');
SELECT style_withpoints('pgr_withpoints', ', ARRAY[2], ARRAY[3])');
SELECT style_withpoints('pgr_withpoints', ', 2, 3, true)');
SELECT style_withpoints('pgr_withpoints', ', 2, ARRAY[3], true)');
SELECT style_withpoints('pgr_withpoints', ', ARRAY[2], 3, true)');
SELECT style_withpoints('pgr_withpoints', ', ARRAY[2], ARRAY[3], true)');
-- UNDIRECTED
SELECT style_withpoints('pgr_withpoints', ', 2, 3, false)');
SELECT style_withpoints('pgr_withpoints', ', 2, ARRAY[3], false)');
SELECT style_withpoints('pgr_withpoints', ', ARRAY[2], 3, false)');
SELECT style_withpoints('pgr_withpoints', ', ARRAY[2], ARRAY[3], false)');



SELECT finish();
ROLLBACK;
