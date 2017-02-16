\i setup.sql

SELECT plan(47);


SELECT has_function('pgr_withpointsksp',
    ARRAY['text', 'text', 'bigint', 'bigint', 'integer', 'boolean', 'boolean', 'character', 'boolean']);

SELECT function_returns('pgr_withpointsksp',
    ARRAY['text', 'text', 'bigint', 'bigint', 'integer', 'boolean', 'boolean', 'character', 'boolean'],
    'setof record');


-- ONE SOURCE
--with reverse cost
SELECT test_anyInteger('pgr_withpointsksp',
    $$, 'SELECT * from pointsOfInterest', 2, 3, 2, true)$$,
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'id');
SELECT test_anyInteger('pgr_withpointsksp',
    $$, 'SELECT * from pointsOfInterest', 2, 3, 2, true)$$,
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'source');
SELECT test_anyInteger('pgr_withpointsksp',
    $$, 'SELECT * from pointsOfInterest', 2, 3, 2, true)$$,
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'target');
SELECT test_anyNumerical('pgr_withpointsksp',
    $$, 'SELECT * from pointsOfInterest', 2, 3, 2, true)$$,
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'cost');
SELECT test_anyNumerical('pgr_withpointsksp',
    $$, 'SELECT * from pointsOfInterest', 2, 3, 2, true)$$,
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'reverse_cost');


--without reverse cost
SELECT test_anyInteger('pgr_withpointsksp',
    $$, 'SELECT * from pointsOfInterest', 2, 3, 2, true)$$,
    ARRAY['id', 'source', 'target', 'cost'],
    'id');
SELECT test_anyInteger('pgr_withpointsksp',
    $$, 'SELECT * from pointsOfInterest', 2, 3, 2, true)$$,
    ARRAY['id', 'source', 'target', 'cost'],
    'source');
SELECT test_anyInteger('pgr_withpointsksp',
    $$, 'SELECT * from pointsOfInterest', 2, 3, 2, true)$$,
    ARRAY['id', 'source', 'target', 'cost'],
    'target');
SELECT test_anyNumerical('pgr_withpointsksp',
    $$, 'SELECT * from pointsOfInterest', 2, 3, 2, true)$$,
    ARRAY['id', 'source', 'target', 'cost'],
    'cost');



SELECT finish();
ROLLBACK;
