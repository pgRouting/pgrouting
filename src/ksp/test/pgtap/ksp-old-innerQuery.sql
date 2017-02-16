\i setup.sql

SELECT plan(47);


SELECT has_function('pgr_ksp',
    ARRAY['text', 'integer', 'integer', 'integer', 'boolean']);

SELECT function_returns('pgr_ksp',
    ARRAY['text', 'integer', 'integer', 'integer', 'boolean'],
    'setof pgr_costresult3');


-- ONE SOURCE
--with reverse cost
SELECT test_integer('pgr_ksp', ',2,3,2, true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'id');
SELECT test_integer('pgr_ksp', ',2,3,2, true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'source');
SELECT test_integer('pgr_ksp', ',2,3,2, true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'target');
SELECT test_float('pgr_ksp', ',2,3,2, true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'cost');
SELECT test_float('pgr_ksp', ',2,3,2, true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'reverse_cost');


--without reverse cost
SELECT test_integer('pgr_ksp', ',2,3,2, false)',
    ARRAY['id', 'source', 'target', 'cost'],
    'id');
SELECT test_integer('pgr_ksp', ',2,3,2, false)',
    ARRAY['id', 'source', 'target', 'cost'],
    'source');
SELECT test_integer('pgr_ksp', ',2,3,2, false)',
    ARRAY['id', 'source', 'target', 'cost'],
    'target');
SELECT test_float('pgr_ksp', ',2,3,2, false)',
    ARRAY['id', 'source', 'target', 'cost'],
    'cost');

SELECT finish();
ROLLBACK;
