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
--with reverse cost
SELECT test_anyInteger('pgr_withpointsdd',
    $$, 'SELECT * from pointsOfInterest', 2, 3, true)$$,
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'id');
SELECT test_anyInteger('pgr_withpointsdd',
    $$, 'SELECT * from pointsOfInterest', 2, 3, true)$$,
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'source');
SELECT test_anyInteger('pgr_withpointsdd',
    $$, 'SELECT * from pointsOfInterest', 2, 3, true)$$,
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'target');
SELECT test_anyNumerical('pgr_withpointsdd',
    $$, 'SELECT * from pointsOfInterest', 2, 3, true)$$,
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'cost');
SELECT test_anyNumerical('pgr_withpointsdd',
    $$, 'SELECT * from pointsOfInterest', 2, 3, true)$$,
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'reverse_cost');


--without reverse cost
SELECT test_anyInteger('pgr_withpointsdd',
    $$, 'SELECT * from pointsOfInterest', 2, 3, true)$$,
    ARRAY['id', 'source', 'target', 'cost'],
    'id');
SELECT test_anyInteger('pgr_withpointsdd',
    $$, 'SELECT * from pointsOfInterest', 2, 3, true)$$,
    ARRAY['id', 'source', 'target', 'cost'],
    'source');
SELECT test_anyInteger('pgr_withpointsdd',
    $$, 'SELECT * from pointsOfInterest', 2, 3, true)$$,
    ARRAY['id', 'source', 'target', 'cost'],
    'target');
SELECT test_anyNumerical('pgr_withpointsdd',
    $$, 'SELECT * from pointsOfInterest', 2, 3, true)$$,
    ARRAY['id', 'source', 'target', 'cost'],
    'cost');



-- MANY SOURCES
--with reverse cost
SELECT test_anyInteger('pgr_withpointsdd',
    $$, 'SELECT * from pointsOfInterest', ARRAY[2], 3, true)$$,
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'id');
SELECT test_anyInteger('pgr_withpointsdd',
    $$, 'SELECT * from pointsOfInterest', ARRAY[2], 3, true)$$,
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'source');
SELECT test_anyInteger('pgr_withpointsdd',
    $$, 'SELECT * from pointsOfInterest', ARRAY[2], 3, true)$$,
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'target');
SELECT test_anyNumerical('pgr_withpointsdd',
    $$, 'SELECT * from pointsOfInterest', ARRAY[2], 3, true)$$,
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'cost');
SELECT test_anyNumerical('pgr_withpointsdd',
    $$, 'SELECT * from pointsOfInterest', ARRAY[2], 3, true)$$,
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'reverse_cost');


--without reverse cost
SELECT test_anyInteger('pgr_withpointsdd',
    $$, 'SELECT * from pointsOfInterest', ARRAY[2], 3, true)$$,
    ARRAY['id', 'source', 'target', 'cost'],
    'id');
SELECT test_anyInteger('pgr_withpointsdd',
    $$, 'SELECT * from pointsOfInterest', ARRAY[2], 3, true)$$,
    ARRAY['id', 'source', 'target', 'cost'],
    'source');
SELECT test_anyInteger('pgr_withpointsdd',
    $$, 'SELECT * from pointsOfInterest', ARRAY[2], 3, true)$$,
    ARRAY['id', 'source', 'target', 'cost'],
    'target');
SELECT test_anyNumerical('pgr_withpointsdd',
    $$, 'SELECT * from pointsOfInterest', ARRAY[2], 3, true)$$,
    ARRAY['id', 'source', 'target', 'cost'],
    'cost');



SELECT finish();
ROLLBACK;
