\i setup.sql

SELECT plan(137);
SET client_min_messages TO ERROR;


SELECT has_function('pgr_dijkstratrsp',
    ARRAY['text', 'text', 'bigint', 'bigint', 'boolean','boolean','boolean']);

SELECT function_returns('pgr_dijkstratrsp',
    ARRAY['text', 'text', 'bigint', 'bigint', 'boolean','boolean','boolean'],
    'setof record');

SELECT style_dijkstraTRSP('pgr_dijkstratrsp', ', $$SELECT * FROM restrict$$, 2, 3)');
SELECT style_dijkstraTRSP('pgr_dijkstratrsp', ', $$SELECT * FROM restrict$$, 2, 3, true)');
SELECT style_dijkstraTRSP('pgr_dijkstratrsp', ', $$SELECT * FROM restrict$$, 2, 3, false)');

SELECT finish();
ROLLBACK;
