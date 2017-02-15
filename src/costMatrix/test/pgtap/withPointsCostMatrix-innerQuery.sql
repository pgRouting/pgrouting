\i setup.sql

SELECT plan(47);
SET client_min_messages TO ERROR;


SELECT has_function('pgr_withpointscostmatrix',
    ARRAY['text', 'text', 'anyarray', 'boolean', 'character']);

SELECT function_returns('pgr_withpointscostmatrix',
    ARRAY['text', 'text', 'anyarray', 'boolean', 'character'],
    'setof record');


-- ONE TO ONE
--with reverse cost
SELECT test_anyInteger('pgr_withpointscostmatrix',
    $$, 'SELECT * from pointsOfInterest',ARRAY[2, 3], true)$$,
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'id');
SELECT test_anyInteger('pgr_withpointscostmatrix',
    $$, 'SELECT * from pointsOfInterest',ARRAY[2, 3], true)$$,
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'source');
SELECT test_anyInteger('pgr_withpointscostmatrix',
    $$, 'SELECT * from pointsOfInterest',ARRAY[2, 3], true)$$,
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'target');
SELECT test_anyNumerical('pgr_withpointscostmatrix',
    $$, 'SELECT * from pointsOfInterest',ARRAY[2, 3], true)$$,
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'cost');
SELECT test_anyNumerical('pgr_withpointscostmatrix',
    $$, 'SELECT * from pointsOfInterest',ARRAY[2, 3], true)$$,
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'reverse_cost');


--without reverse cost
SELECT test_anyInteger('pgr_withpointscostmatrix',
    $$, 'SELECT * from pointsOfInterest',ARRAY[2, 3], true)$$,
    ARRAY['id', 'source', 'target', 'cost'],
    'id');
SELECT test_anyInteger('pgr_withpointscostmatrix',
    $$, 'SELECT * from pointsOfInterest',ARRAY[2, 3], true)$$,
    ARRAY['id', 'source', 'target', 'cost'],
    'source');
SELECT test_anyInteger('pgr_withpointscostmatrix',
    $$, 'SELECT * from pointsOfInterest',ARRAY[2, 3], true)$$,
    ARRAY['id', 'source', 'target', 'cost'],
    'target');
SELECT test_anyNumerical('pgr_withpointscostmatrix',
    $$, 'SELECT * from pointsOfInterest',ARRAY[2, 3], true)$$,
    ARRAY['id', 'source', 'target', 'cost'],
    'cost');



SELECT finish();
ROLLBACK;
