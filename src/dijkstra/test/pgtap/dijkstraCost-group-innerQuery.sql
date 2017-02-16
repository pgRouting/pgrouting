\i setup.sql

SELECT plan(188);
SET client_min_messages TO ERROR;


SELECT has_function('pgr_dijkstracost',
    ARRAY['text', 'bigint', 'bigint', 'boolean']);
SELECT has_function('pgr_dijkstracost',
    ARRAY['text', 'bigint', 'anyarray', 'boolean']);
SELECT has_function('pgr_dijkstracost',
    ARRAY['text', 'anyarray', 'bigint', 'boolean']);
SELECT has_function('pgr_dijkstracost',
    ARRAY['text', 'anyarray', 'anyarray', 'boolean']);

SELECT function_returns('pgr_dijkstracost',
    ARRAY['text', 'bigint', 'bigint', 'boolean'],
    'setof record');
SELECT function_returns('pgr_dijkstracost',
    ARRAY['text', 'bigint', 'anyarray', 'boolean'],
    'setof record');
SELECT function_returns('pgr_dijkstracost',
    ARRAY['text', 'anyarray', 'bigint', 'boolean'],
    'setof record');
SELECT function_returns('pgr_dijkstracost',
    ARRAY['text', 'anyarray', 'anyarray', 'boolean'],
    'setof record');


-- ONE TO ONE
--with reverse cost
SELECT test_anyInteger('pgr_dijkstracost', ',2,3, true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'id');
SELECT test_anyInteger('pgr_dijkstracost', ',2,3, true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'source');
SELECT test_anyInteger('pgr_dijkstracost', ',2,3, true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'target');
SELECT test_anyNumerical('pgr_dijkstracost', ',2,3, true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'cost');
SELECT test_anyNumerical('pgr_dijkstracost', ',2,3, true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'reverse_cost');


--without reverse cost
SELECT test_anyInteger('pgr_dijkstracost', ',2,3, true)',
    ARRAY['id', 'source', 'target', 'cost'],
    'id');
SELECT test_anyInteger('pgr_dijkstracost', ',2,3, true)',
    ARRAY['id', 'source', 'target', 'cost'],
    'source');
SELECT test_anyInteger('pgr_dijkstracost', ',2,3, true)',
    ARRAY['id', 'source', 'target', 'cost'],
    'target');
SELECT test_anyNumerical('pgr_dijkstracost', ',2,3, true)',
    ARRAY['id', 'source', 'target', 'cost'],
    'cost');



-- ONE TO MANY
--with reverse cost
SELECT test_anyInteger('pgr_dijkstracost', ',2,ARRAY[3], true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'id');
SELECT test_anyInteger('pgr_dijkstracost', ',2,ARRAY[3], true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'source');
SELECT test_anyInteger('pgr_dijkstracost', ',2,ARRAY[3], true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'target');
SELECT test_anyNumerical('pgr_dijkstracost', ',2,ARRAY[3], true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'cost');
SELECT test_anyNumerical('pgr_dijkstracost', ',2,ARRAY[3], true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'reverse_cost');


--without reverse cost
SELECT test_anyInteger('pgr_dijkstracost', ',2,ARRAY[3], true)',
    ARRAY['id', 'source', 'target', 'cost'],
    'id');
SELECT test_anyInteger('pgr_dijkstracost', ',2,ARRAY[3], true)',
    ARRAY['id', 'source', 'target', 'cost'],
    'source');
SELECT test_anyInteger('pgr_dijkstracost', ',2,ARRAY[3], true)',
    ARRAY['id', 'source', 'target', 'cost'],
    'target');
SELECT test_anyNumerical('pgr_dijkstracost', ',2,ARRAY[3], true)',
    ARRAY['id', 'source', 'target', 'cost'],
    'cost');



-- MANY TO ONE
--with reverse cost
SELECT test_anyInteger('pgr_dijkstracost', ',ARRAY[2],3, true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'id');
SELECT test_anyInteger('pgr_dijkstracost', ',ARRAY[2],3, true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'source');
SELECT test_anyInteger('pgr_dijkstracost', ',ARRAY[2],3, true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'target');
SELECT test_anyNumerical('pgr_dijkstracost', ',ARRAY[2],3, true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'cost');
SELECT test_anyNumerical('pgr_dijkstracost', ',ARRAY[2],3, true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'reverse_cost');


--without reverse cost
SELECT test_anyInteger('pgr_dijkstracost', ',ARRAY[2],3, true)',
    ARRAY['id', 'source', 'target', 'cost'],
    'id');
SELECT test_anyInteger('pgr_dijkstracost', ',ARRAY[2],3, true)',
    ARRAY['id', 'source', 'target', 'cost'],
    'source');
SELECT test_anyInteger('pgr_dijkstracost', ',ARRAY[2],3, true)',
    ARRAY['id', 'source', 'target', 'cost'],
    'target');
SELECT test_anyNumerical('pgr_dijkstracost', ',ARRAY[2],3, true)',
    ARRAY['id', 'source', 'target', 'cost'],
    'cost');



-- MANY TO MANY
--with reverse cost
SELECT test_anyInteger('pgr_dijkstracost', ',ARRAY[2],ARRAY[3], true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'id');
SELECT test_anyInteger('pgr_dijkstracost', ',ARRAY[2],ARRAY[3], true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'source');
SELECT test_anyInteger('pgr_dijkstracost', ',ARRAY[2],ARRAY[3], true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'target');
SELECT test_anyNumerical('pgr_dijkstracost', ',ARRAY[2],ARRAY[3], true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'cost');
SELECT test_anyNumerical('pgr_dijkstracost', ',ARRAY[2],ARRAY[3], true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'reverse_cost');


--without reverse cost
SELECT test_anyInteger('pgr_dijkstracost', ',ARRAY[2],ARRAY[3], true)',
    ARRAY['id', 'source', 'target', 'cost'],
    'id');
SELECT test_anyInteger('pgr_dijkstracost', ',ARRAY[2],ARRAY[3], true)',
    ARRAY['id', 'source', 'target', 'cost'],
    'source');
SELECT test_anyInteger('pgr_dijkstracost', ',ARRAY[2],ARRAY[3], true)',
    ARRAY['id', 'source', 'target', 'cost'],
    'target');
SELECT test_anyNumerical('pgr_dijkstracost', ',ARRAY[2],ARRAY[3], true)',
    ARRAY['id', 'source', 'target', 'cost'],
    'cost');



SELECT finish();
ROLLBACK;
