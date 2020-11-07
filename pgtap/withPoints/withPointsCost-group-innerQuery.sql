\i setup.sql

UPDATE edge_table SET cost = sign(cost), reverse_cost = sign(reverse_cost);
SELECT plan(658);
SET client_min_messages TO ERROR;


SELECT has_function('pgr_withpointscost',
    ARRAY['text', 'text', 'bigint', 'bigint', 'boolean','character']);
SELECT has_function('pgr_withpointscost',
    ARRAY['text', 'text', 'bigint', 'anyarray', 'boolean','character']);
SELECT has_function('pgr_withpointscost',
    ARRAY['text', 'text', 'anyarray', 'bigint', 'boolean','character']);
SELECT has_function('pgr_withpointscost',
    ARRAY['text', 'text', 'anyarray', 'anyarray', 'boolean','character']);
SELECT has_function('pgr_withpointscost',
    ARRAY['text', 'text', 'text', 'boolean','character']);

SELECT function_returns('pgr_withpointscost',
    ARRAY['text', 'text', 'bigint', 'bigint', 'boolean','character'],
    'setof record');
SELECT function_returns('pgr_withpointscost',
    ARRAY['text', 'text', 'bigint', 'anyarray', 'boolean','character'],
    'setof record');
SELECT function_returns('pgr_withpointscost',
    ARRAY['text', 'text', 'anyarray', 'bigint', 'boolean','character'],
    'setof record');
SELECT function_returns('pgr_withpointscost',
    ARRAY['text', 'text', 'anyarray', 'anyarray', 'boolean','character'],
    'setof record');
SELECT function_returns('pgr_withpointscost',
    ARRAY['text', 'text', 'text', 'boolean','character'],
    'setof record');


-- DIRECTED
SELECT style_withpoints('pgr_withpointscost', ', 2, 3)');
SELECT style_withpoints('pgr_withpointscost', ', 2, ARRAY[3])');
SELECT style_withpoints('pgr_withpointscost', ', ARRAY[2], 3)');
SELECT style_withpoints('pgr_withpointscost', ', ARRAY[2], ARRAY[3])');
SELECT style_withpoints('pgr_withpointscost', ', 2, 3, true)');
SELECT style_withpoints('pgr_withpointscost', ', 2, ARRAY[3], true)');
SELECT style_withpoints('pgr_withpointscost', ', ARRAY[2], 3, true)');
SELECT style_withpoints('pgr_withpointscost', ', ARRAY[2], ARRAY[3], true)');
-- UNDIRECTED
SELECT style_withpoints('pgr_withpointscost', ', 2, 3, false)');
SELECT style_withpoints('pgr_withpointscost', ', 2, ARRAY[3], false)');
SELECT style_withpoints('pgr_withpointscost', ', ARRAY[2], 3, false)');
SELECT style_withpoints('pgr_withpointscost', ', ARRAY[2], ARRAY[3], false)');


SELECT finish();
ROLLBACK;
