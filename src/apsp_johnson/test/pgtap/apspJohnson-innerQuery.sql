\i setup.sql

SELECT plan(17);
SET client_min_messages TO ERROR;


SELECT has_function('pgr_apspjohnson',
    ARRAY['text']);

SELECT function_returns('pgr_apspjohnson',
    ARRAY['text'],
    'setof pgr_costresult');


-- only works with source, target, cost
-- no reverse_cost and no id
SELECT test_integer('pgr_apspjohnson', ')',
    ARRAY['source', 'target', 'cost'],
    'source');
SELECT test_integer('pgr_apspjohnson', ')',
    ARRAY['source', 'target', 'cost'],
    'target');
SELECT test_float('pgr_apspjohnson', ')',
    ARRAY['source', 'target', 'cost'],
    'cost');



SELECT finish();
ROLLBACK;
