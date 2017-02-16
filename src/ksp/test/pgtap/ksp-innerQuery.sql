\i setup.sql

SELECT plan(47);


SELECT has_function('pgr_ksp',
    ARRAY['text', 'bigint', 'bigint', 'integer', 'boolean', 'boolean']);

SELECT function_returns('pgr_ksp',
    ARRAY['text', 'bigint', 'bigint', 'integer', 'boolean', 'boolean'],
    'setof record');


-- ONE SOURCE
--with reverse cost
SELECT test_anyInteger('pgr_ksp', ',2,3,2, true, false)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'id');
SELECT test_anyInteger('pgr_ksp', ',2,3,2, true, false)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'source');
SELECT test_anyInteger('pgr_ksp', ',2,3,2, true, false)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'target');
SELECT test_anyNumerical('pgr_ksp', ',2,3,2, true, false)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'cost');
SELECT test_anyNumerical('pgr_ksp', ',2,3,2, true, false)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'reverse_cost');


--without reverse cost
SELECT test_anyInteger('pgr_ksp', ',2,3,2, true, false)',
    ARRAY['id', 'source', 'target', 'cost'],
    'id');
SELECT test_anyInteger('pgr_ksp', ',2,3,2, true, false)',
    ARRAY['id', 'source', 'target', 'cost'],
    'source');
SELECT test_anyInteger('pgr_ksp', ',2,3,2, true, false)',
    ARRAY['id', 'source', 'target', 'cost'],
    'target');
SELECT test_anyNumerical('pgr_ksp', ',2,3,2, true, false)',
    ARRAY['id', 'source', 'target', 'cost'],
    'cost');


SELECT finish();
ROLLBACK;
