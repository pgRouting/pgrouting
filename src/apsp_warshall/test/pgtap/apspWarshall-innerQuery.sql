\i setup.sql

SELECT plan(47);
SET client_min_messages TO ERROR;


SELECT has_function('pgr_apspwarshall',
    ARRAY['text', 'boolean', 'boolean']);

SELECT function_returns('pgr_apspwarshall',
    ARRAY['text', 'boolean', 'boolean'],
    'setof pgr_costresult');


--with reverse cost
SELECT test_integer('pgr_apspwarshall', ', true, true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'id');
SELECT test_integer('pgr_apspwarshall', ', true, true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'source');
SELECT test_integer('pgr_apspwarshall', ', true, true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'target');
SELECT test_float('pgr_apspwarshall', ', true, true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'cost');
SELECT test_float('pgr_apspwarshall', ', true, true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'reverse_cost');


--without reverse cost
SELECT test_integer('pgr_apspwarshall', ', true, false)',
    ARRAY['id', 'source', 'target', 'cost'],
    'id');
SELECT test_integer('pgr_apspwarshall', ', true, false)',
    ARRAY['id', 'source', 'target', 'cost'],
    'source');
SELECT test_integer('pgr_apspwarshall', ', true, false)',
    ARRAY['id', 'source', 'target', 'cost'],
    'target');
SELECT test_float('pgr_apspwarshall', ', true, false)',
    ARRAY['id', 'source', 'target', 'cost'],
    'cost');



SELECT finish();
ROLLBACK;
