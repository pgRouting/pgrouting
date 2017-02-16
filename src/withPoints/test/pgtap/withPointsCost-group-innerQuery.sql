\i setup.sql

SELECT plan(188);
SET client_min_messages TO ERROR;


SELECT has_function('pgr_withpointscost',
    ARRAY['text', 'text', 'bigint', 'bigint', 'boolean','character']);
SELECT has_function('pgr_withpointscost',
    ARRAY['text', 'text', 'bigint', 'anyarray', 'boolean','character']);
SELECT has_function('pgr_withpointscost',
    ARRAY['text', 'text', 'anyarray', 'bigint', 'boolean','character']);
SELECT has_function('pgr_withpointscost',
    ARRAY['text', 'text', 'anyarray', 'anyarray', 'boolean','character']);

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


-- ONE TO ONE
--with reverse cost
SELECT test_anyInteger('pgr_withpointscost',
    $$, 'SELECT * from pointsOfInterest', 2, 3, true)$$,
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'id');
SELECT test_anyInteger('pgr_withpointscost',
    $$, 'SELECT * from pointsOfInterest', 2, 3, true)$$,
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'source');
SELECT test_anyInteger('pgr_withpointscost',
    $$, 'SELECT * from pointsOfInterest', 2, 3, true)$$,
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'target');
SELECT test_anyNumerical('pgr_withpointscost',
    $$, 'SELECT * from pointsOfInterest', 2, 3, true)$$,
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'cost');
SELECT test_anyNumerical('pgr_withpointscost',
    $$, 'SELECT * from pointsOfInterest', 2, 3, true)$$,
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'reverse_cost');


--without reverse cost
SELECT test_anyInteger('pgr_withpointscost',
    $$, 'SELECT * from pointsOfInterest', 2, 3, true)$$,
    ARRAY['id', 'source', 'target', 'cost'],
    'id');
SELECT test_anyInteger('pgr_withpointscost',
    $$, 'SELECT * from pointsOfInterest', 2, 3, true)$$,
    ARRAY['id', 'source', 'target', 'cost'],
    'source');
SELECT test_anyInteger('pgr_withpointscost',
    $$, 'SELECT * from pointsOfInterest', 2, 3, true)$$,
    ARRAY['id', 'source', 'target', 'cost'],
    'target');
SELECT test_anyNumerical('pgr_withpointscost',
    $$, 'SELECT * from pointsOfInterest', 2, 3, true)$$,
    ARRAY['id', 'source', 'target', 'cost'],
    'cost');



-- ONE TO MANY
--with reverse cost
SELECT test_anyInteger('pgr_withpointscost', 
    $$, 'SELECT * from pointsOfInterest', 2, ARRAY[3], true)$$,
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'id');
SELECT test_anyInteger('pgr_withpointscost',
    $$, 'SELECT * from pointsOfInterest', 2, ARRAY[3], true)$$,
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'source');
SELECT test_anyInteger('pgr_withpointscost',
    $$, 'SELECT * from pointsOfInterest', 2, ARRAY[3], true)$$,
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'target');
SELECT test_anyNumerical('pgr_withpointscost',
    $$, 'SELECT * from pointsOfInterest', 2, ARRAY[3], true)$$,
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'cost');
SELECT test_anyNumerical('pgr_withpointscost',
    $$, 'SELECT * from pointsOfInterest', 2, ARRAY[3], true)$$,
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'reverse_cost');


--without reverse cost
SELECT test_anyInteger('pgr_withpointscost',
    $$, 'SELECT * from pointsOfInterest', 2, ARRAY[3], true)$$,
    ARRAY['id', 'source', 'target', 'cost'],
    'id');
SELECT test_anyInteger('pgr_withpointscost',
    $$, 'SELECT * from pointsOfInterest', 2, ARRAY[3], true)$$,
    ARRAY['id', 'source', 'target', 'cost'],
    'source');
SELECT test_anyInteger('pgr_withpointscost',
    $$, 'SELECT * from pointsOfInterest', 2, ARRAY[3], true)$$,
    ARRAY['id', 'source', 'target', 'cost'],
    'target');
SELECT test_anyNumerical('pgr_withpointscost',
    $$, 'SELECT * from pointsOfInterest', 2, ARRAY[3], true)$$,
    ARRAY['id', 'source', 'target', 'cost'],
    'cost');



-- MANY TO ONE
--with reverse cost
SELECT test_anyInteger('pgr_withpointscost',
    $$, 'SELECT * from pointsOfInterest', ARRAY[2], 3, true)$$,
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'id');
SELECT test_anyInteger('pgr_withpointscost',
    $$, 'SELECT * from pointsOfInterest', ARRAY[2], 3, true)$$,
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'source');
SELECT test_anyInteger('pgr_withpointscost',
    $$, 'SELECT * from pointsOfInterest', ARRAY[2], 3, true)$$,
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'target');
SELECT test_anyNumerical('pgr_withpointscost',
    $$, 'SELECT * from pointsOfInterest', ARRAY[2], 3, true)$$,
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'cost');
SELECT test_anyNumerical('pgr_withpointscost',
    $$, 'SELECT * from pointsOfInterest', ARRAY[2], 3, true)$$,
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'reverse_cost');


--without reverse cost
SELECT test_anyInteger('pgr_withpointscost',
    $$, 'SELECT * from pointsOfInterest', ARRAY[2], 3, true)$$,
    ARRAY['id', 'source', 'target', 'cost'],
    'id');
SELECT test_anyInteger('pgr_withpointscost',
    $$, 'SELECT * from pointsOfInterest', ARRAY[2], 3, true)$$,
    ARRAY['id', 'source', 'target', 'cost'],
    'source');
SELECT test_anyInteger('pgr_withpointscost',
    $$, 'SELECT * from pointsOfInterest', ARRAY[2], 3, true)$$,
    ARRAY['id', 'source', 'target', 'cost'],
    'target');
SELECT test_anyNumerical('pgr_withpointscost',
    $$, 'SELECT * from pointsOfInterest', ARRAY[2], 3, true)$$,
    ARRAY['id', 'source', 'target', 'cost'],
    'cost');



-- MANY TO MANY
--with reverse cost
SELECT test_anyInteger('pgr_withpointscost',
    $$, 'SELECT * from pointsOfInterest', ARRAY[2], ARRAY[3], true)$$,
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'id');
SELECT test_anyInteger('pgr_withpointscost',
    $$, 'SELECT * from pointsOfInterest', ARRAY[2], ARRAY[3], true)$$,
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'source');
SELECT test_anyInteger('pgr_withpointscost',
    $$, 'SELECT * from pointsOfInterest', ARRAY[2], ARRAY[3], true)$$,
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'target');
SELECT test_anyNumerical('pgr_withpointscost',
    $$, 'SELECT * from pointsOfInterest', ARRAY[2], ARRAY[3], true)$$,
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'cost');
SELECT test_anyNumerical('pgr_withpointscost',
    $$, 'SELECT * from pointsOfInterest', ARRAY[2], ARRAY[3], true)$$,
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'reverse_cost');


--without reverse cost
SELECT test_anyInteger('pgr_withpointscost',
    $$, 'SELECT * from pointsOfInterest', ARRAY[2], ARRAY[3], true)$$,
    ARRAY['id', 'source', 'target', 'cost'],
    'id');
SELECT test_anyInteger('pgr_withpointscost',
    $$, 'SELECT * from pointsOfInterest', ARRAY[2], ARRAY[3], true)$$,
    ARRAY['id', 'source', 'target', 'cost'],
    'source');
SELECT test_anyInteger('pgr_withpointscost',
    $$, 'SELECT * from pointsOfInterest', ARRAY[2], ARRAY[3], true)$$,
    ARRAY['id', 'source', 'target', 'cost'],
    'target');
SELECT test_anyNumerical('pgr_withpointscost',
    $$, 'SELECT * from pointsOfInterest', ARRAY[2], ARRAY[3], true)$$,
    ARRAY['id', 'source', 'target', 'cost'],
    'cost');



SELECT finish();
ROLLBACK;
